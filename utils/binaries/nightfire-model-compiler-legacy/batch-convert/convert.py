import sys
import os
import subprocess
import shutil
import struct
import threading
import inspect
import math

from concurrent.futures.thread import ThreadPoolExecutor

NUM_THREADS = 4

SCRIPT_DIR = os.path.dirname(os.path.realpath(sys.argv[0]))
INPUT_DIR = os.path.join(SCRIPT_DIR, "v14")
INPUT_TEXTURE_DIR = os.path.join(INPUT_DIR, "textures")
OUTPUT_DIR = os.path.join(SCRIPT_DIR, "v10")
SCRATCH_DIR = os.path.join(SCRIPT_DIR, "scratch")
TEXTURE_DIR_NAME = "mdl"

MDLCONVERT_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "mdlconvert.exe"))
STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "abstudiomdl.exe"))
ERROR_SUMMARY_PATH = os.path.join(SCRIPT_DIR, "errors.log")

MDL_HEADER_FORMAT = "II64sIfffffffffffffffIIIIIIIIIIIIIIIIIIIIIIIIIII"
MDL_TEXTURE_FORMAT = "64sIIII"
MDL_TEXTURE_STRUCT_SIZE = struct.calcsize(MDL_TEXTURE_FORMAT)
MDL_BODYPART_FORMAT = "64sIII"
MDL_BODYPART_STRUCT_SIZE = struct.calcsize(MDL_BODYPART_FORMAT)
MDL_MODEL_FORMAT = "64sIfIIIIIIIIII"
MDL_MODEL_STRUCT_SIZE = struct.calcsize(MDL_MODEL_FORMAT)
MDL_MESH_FORMAT = "IIIII"
MDL_MESH_STRUCT_SIZE = struct.calcsize(MDL_MESH_FORMAT)
MDL_SHORT_FORMAT = "h"
MDL_SHORT_STRUCT_SIZE = struct.calcsize(MDL_SHORT_FORMAT)
MDL_TRICMD_FORMAT = "hhhh"
MDL_TRICMD_STRUCT_SIZE = struct.calcsize(MDL_TRICMD_FORMAT)

STUDIO_NO_EMBEDDED_TEXTURES = 0x800

# Map from all-lowercase texture names to actual names.
# This is needed to properly update texture names in the
# MDL file, or the engine won't be able to find the textures.
TextureLookup = {}

def relPath(path:str):
	return os.path.relpath(path, SCRIPT_DIR)

class FileProcessor:
	def __init__(self, filePath):
		self.filePath = filePath
		self.fileScratchDir = ""
		self.referencedTextures = {}
		self.referenceSmds = []
		self.boundsForTexture = {}

	def processInputFile(self):
		inputMdlRelPath = os.path.relpath(self.filePath, INPUT_DIR)
		inputMdlRelPathNoExt = os.path.splitext(inputMdlRelPath)[0]

		self.fileScratchDir = os.path.join(SCRATCH_DIR, inputMdlRelPathNoExt)
		os.makedirs(self.fileScratchDir, exist_ok=True)

		# Dump the selected model to the scratch dir.
		qcPath = self.dumpToQc(self.filePath, self.fileScratchDir)

		# Determine which reference SMDs we have.
		self.referenceSmds = self.findReferenceSmds(qcPath)

		# Fix all texture paths in all SMDs.
		self.fixSmdFiles(self.fileScratchDir)
		textures = list(self.referencedTextures.keys())

		# Create fake textures somewhere that StudioMDL can see them.
		self.copyTextures(textures, INPUT_TEXTURE_DIR, os.path.join(self.fileScratchDir, TEXTURE_DIR_NAME))

		# Compile the QC file.
		self.compileQc(qcPath)

		# Copy the compiler model to the target directory.
		mdlPath = os.path.splitext(qcPath)[0] + ".mdl"
		self.copyMdlToOutput(mdlPath, os.path.join(OUTPUT_DIR, inputMdlRelPath))

	def dumpToQc(self, inputFile:str, outputDir:str):
		fileName = os.path.splitext(os.path.basename(inputFile))[0]
		outputPath = os.path.join(outputDir, f"{fileName}.qc")

		args = \
		[
			MDLCONVERT_PATH,
			"-dumpv14tov10",
			inputFile,
			outputPath
		]

		self.runCommand(args, output="DumpToQC")
		return outputPath

	def findReferenceSmds(self, qcPath:str):
		lines = []
		referencedSmds = []
		inBodyGroup = False

		self.logMsg("Finding reference SMDs from", relPath(qcPath), file="FindReferenceSmds")

		with open(qcPath, "r") as inFile:
			lines = [line.strip() for line in inFile.readlines()]

		for line in lines:
			if line.startswith("$bodygroup"):
				if inBodyGroup:
					raise RuntimeError(f"{relPath(qcPath)} had invalid nested body group.")
				inBodyGroup = True
			elif inBodyGroup:
				if line == "}":
					inBodyGroup = False
				else:
					segments = line.split(maxsplit=1)

					if len(segments) == 2 and segments[0] == "studio":
						# We assume that anything after the "studio" is the reference SMD
						smdPath = segments[1].strip('"') + ".smd"

						self.logMsg("Found SMD:", smdPath, file="FindReferenceSmds")
						referencedSmds.append(smdPath)

		return referencedSmds

	def fixSmdFiles(self, dirPath:str):
		for smdFile in os.listdir(dirPath):
			ext = os.path.splitext(smdFile)[1].lower()

			if ext != ".smd":
				continue

			self.processSmdFile(os.path.abspath(os.path.join(dirPath, smdFile)))

	def processSmdFile(self, smdPath:str):
		lines = []

		with open(smdPath, "r") as inFile:
			lines = [line.rstrip() for line in inFile.readlines()]

		self.fixSmdTextures(smdPath, lines)

		with open(smdPath, "w") as outFile:
			# The final \n is required or studiomdl freaks out
			outFile.write("\n".join(lines) + "\n")

	def fixSmdTextures(self, smdPath:str, lines:list):
		global TextureLookup

		begunTriangles = False

		self.logMsg("Fixing referenced textures for", relPath(smdPath), file="FixSMDTextures")

		for index in range(0, len(lines)):
			line = lines[index]

			if line == "triangles":
				begunTriangles = True
				continue

			if not begunTriangles:
				continue

			segments = os.path.splitext(line)

			if len(segments) != 2 or segments[1] != ".bmp":
				continue

			textureName = line
			pngName = segments[0].lower() + ".png"

			if pngName not in TextureLookup:
				raise RuntimeError(f"{relPath(smdPath)} contains unknown texture {textureName}")

			if textureName not in self.referencedTextures:
				self.logMsg(f"Texture referenced: {textureName} ({pngName})", file="FixSMDTextures")

				# Record that we encountered this texture.
				self.referencedTextures[textureName] = True

			# Rename each bmp file to an appropriate png, namespaced in an "mdl" folder
			lines[index] = f"{TEXTURE_DIR_NAME}/{TextureLookup[pngName]}"

	def copyTextures(self, textures:list, inputDir:str, outputDir:str):
		global TextureLookup

		if not os.path.isdir(outputDir):
			os.makedirs(outputDir, exist_ok=True)

		for texture in textures:
			textureFileNameOnDisk = TextureLookup[os.path.splitext(texture)[0].lower() + ".png"]

			inputPath = os.path.join(inputDir, textureFileNameOnDisk)
			outputPath = os.path.join(outputDir, textureFileNameOnDisk)

			self.logMsg("Copying texture", relPath(inputPath), "to", relPath(outputPath), file="CopyTextures")

			try:
				shutil.copy2(inputPath, outputPath)
			except Exception as ex:
				raise RuntimeError(f"Encountered error when copying texture {relPath(inputPath)} to {relPath(outputPath)}. {str(ex)}")

	def compileQc(self, qcPath:str):
		baseName = os.path.basename(qcPath)
		workingDir = os.path.dirname(qcPath)
		currentDir = os.curdir

		os.chdir(workingDir)

		args = \
		[
			STUDIOMDL_PATH,
			"-e",
			baseName
		]

		self.runCommand(args, output="CompileQC")

		os.chdir(currentDir)

	def copyMdlToOutput(self, source:str, dest:str):
		self.logMsg("Copying", relPath(source), "to", relPath(dest), file="CopyMdlToOutput")

		try:
			os.makedirs(os.path.dirname(dest), exist_ok=True)
			shutil.copy2(source, dest)
		except Exception as ex:
				raise RuntimeError(f"Encountered error when copying MDL file {relPath(source)} to {relPath(dest)}. {str(ex)}")

	def runCommand(self, args, output=""):
		self.logMsg("*** Running command:", *args, file=(output if output else None))

		if output:
			outFilePath = os.path.join(self.fileScratchDir, output + ".log")
			with open(outFilePath, "a+") as stdOut:
				result = subprocess.run(args, shell=True, stdout=stdOut)
		else:
			result = subprocess.run(args, shell=True)

		if result.returncode != 0:
			raise RuntimeError(f"Command {' '.join(args)} returned error code {result.returncode}")

		self.logMsg("*** Command complete.", file=(output if output else None))

	def logMsg(self, *args, file=None):
		if isinstance(file, str):
			with open(os.path.join(self.fileScratchDir, file + ".log"), "a+") as outFile:
				print(*args, file=outFile)
		elif file is not None:
			print(*args, file=file)
		else:
			print(*args)

def validateDirs():
	if not os.path.isdir(INPUT_DIR):
		print("Input directory", INPUT_DIR, "does not exist.", file=sys.stderr)
		sys.exit(1)

	if not os.path.isfile(MDLCONVERT_PATH):
		print("MDL converter", MDLCONVERT_PATH, "does not exist.", file=sys.stderr)
		sys.exit(1)

	if not os.path.isfile(STUDIOMDL_PATH):
		print("StudioMDL compiler", STUDIOMDL_PATH, "does not exist.", file=sys.stderr)
		sys.exit(1)

	if not os.path.isdir(OUTPUT_DIR):
		os.makedirs(OUTPUT_DIR, exist_ok=True)

def buildTextureLookup(textureDir:str):
	global TextureLookup

	TextureLookup = {}

	if not os.path.isdir(textureDir):
		print("Texture directory", textureDir, "was not valid when building texture lookup.", file=sys.stderr)
		sys.exit(1)

	print("Building texture lookup table")

	for texFile in os.listdir(textureDir):
		if os.path.splitext(texFile)[1].lower() != ".png":
			continue

		if texFile.lower() in TextureLookup:
			print("Duplicate texture", texFile, "with different filename case found in textures directory!", file=sys.stderr)
			sys.exit(1)

		TextureLookup[texFile.lower()] = texFile

def cleanScratchDir():
	print("Cleaning", SCRATCH_DIR)

	if os.path.isfile(SCRATCH_DIR):
		os.remove(SCRATCH_DIR)
	elif os.path.isdir(SCRATCH_DIR):
		shutil.rmtree(SCRATCH_DIR)

	os.makedirs(SCRATCH_DIR, exist_ok=True)

def threadTask(files:dict, filePath:str):
	try:
		print("Processing file:", relPath(filePath))
		FileProcessor(filePath).processInputFile()
		files[filePath] = None
	except Exception as ex:
		details = inspect.trace()[-1]
		errorString = f"Exception from {details.filename}:{details.lineno}: {str(ex)}"
		files[filePath] = errorString
		print("***", errorString, file=sys.stderr)
		print("*** An error occurred, skipping file", relPath(filePath), file=sys.stderr)

def getInputFiles(rootDir:str):
	filesFound = []

	for fileEntry in os.listdir(rootDir):
		fullPath = os.path.abspath(os.path.join(rootDir, fileEntry))

		if os.path.isdir(fullPath):
			filesFound += getInputFiles(fullPath)
			continue

		fileExt = os.path.splitext(fileEntry)[1].lower()

		if fileExt != ".mdl":
			continue

		filesFound.append(fullPath)

	return filesFound

def main():
	validateDirs()
	buildTextureLookup(INPUT_TEXTURE_DIR)
	cleanScratchDir()

	filesToProcess = getInputFiles(INPUT_DIR)
	print("Found", len(filesToProcess), "input files")

	filesMap = {filePath: f"{filePath} has not yet been processed." for filePath in filesToProcess}
	threadWork = [(filesMap, filePath) for filePath in filesMap.keys()]

	print("Starting thread pool with", NUM_THREADS, "threads")

	with ThreadPoolExecutor(max_workers=NUM_THREADS) as executor:
		executor.map(lambda args: threadTask(*args), threadWork)

	filesProcessed = 0

	print("Writing error summary...")

	with open(ERROR_SUMMARY_PATH, "w") as errorLogFile:
		for filePath in filesMap:
			if filesMap[filePath] is None:
				filesProcessed += 1
			else:
				errorLogFile.write(filesMap[filePath] + "\n")

	print("Processed", filesProcessed, "of", len(filesToProcess), "files.")
	print("Any processing errors have been logged to", ERROR_SUMMARY_PATH)

if __name__ == "__main__":
	main()
