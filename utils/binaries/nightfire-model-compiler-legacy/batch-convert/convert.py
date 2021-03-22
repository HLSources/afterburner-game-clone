import sys
import os
import subprocess
import shutil
import struct
import threading

from concurrent.futures.thread import ThreadPoolExecutor
from PIL import Image

NUM_THREADS = 1

SCRIPT_DIR = os.path.dirname(os.path.realpath(sys.argv[0]))
INPUT_DIR = os.path.join(SCRIPT_DIR, "v14")
INPUT_TEXTURE_DIR = os.path.join(INPUT_DIR, "textures")
OUTPUT_DIR = os.path.join(SCRIPT_DIR, "v10")
SCRATCH_DIR = os.path.join(SCRIPT_DIR, "scratch")
TEXTURE_DIR_NAME = "mdl"

MDLCONVERT_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "mdlconvert.exe"))
STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "studiomdl_new.exe"))

MDL_HEADER_FORMAT = "II64sIfffffffffffffffIIIIIIIIIIIIIIIIIIIIIIIIIII"
MDL_TEXTURE_FORMAT = "64sIIII"
MDL_TEXTURE_STRUCT_SIZE = struct.calcsize(MDL_TEXTURE_FORMAT)
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

		self.logUVBounds()

		# Create fake textures somewhere that StudioMDL can see them.
		self.createFakeTextures(textures, os.path.join(self.fileScratchDir, TEXTURE_DIR_NAME))

		# Compile the QC file.
		self.compileQc(qcPath)

		# Patch the texture paths so that they point to PNGs.
		mdlPath = os.path.splitext(qcPath)[0] + ".mdl"
		self.patchMdl(mdlPath)

		# Copy the patched model to the target directory.
		self.copyPatchedMdl(mdlPath, os.path.join(OUTPUT_DIR, inputMdlRelPath))

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

		if os.path.basename(smdPath) in self.referenceSmds:
			self.calculateUVBoundsForTextures(smdPath, lines)

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

			segments = line.split(".")

			if len(segments) != 2 or segments[1] != "bmp":
				continue

			textureName = line

			if (segments[0].lower() + ".png") not in TextureLookup:
				raise RuntimeError(f"{relPath(smdPath)} contains unknown texture {textureName}")

			if textureName not in self.referencedTextures:
				self.logMsg("Texture referenced:", textureName, file="FixSMDTextures")

				# Record that we encountered this texture.
				self.referencedTextures[textureName] = True

			# Rename each bmp file to an appropriate png, namespaced in an "mdl" folder
			lines[index] = f"{TEXTURE_DIR_NAME}/{textureName}"

	def calculateUVBoundsForTextures(self, smdPath:str, lines:list):
		trianglesLineIndex = -1
		currentTexture = ""

		self.logMsg("Checking UV bounds for all textures in", relPath(smdPath), file="CalculateUVBoundsForTextures")

		for index in range(0, len(lines)):
			line = lines[index]

			if line == "triangles":
				trianglesLineIndex = index
				self.logMsg("Triangles begin from line", trianglesLineIndex + 2, file="CalculateUVBoundsForTextures")
				continue

			if trianglesLineIndex < 0:
				continue

			if line == "end":
				break

			# Lines go:

			# texturePath
			#   vertex 1
			#   vertex 2
			#   vertex 3
			# ... repeat ...

			# We only care about lines with vertices.

			moduloIndex = (index - (trianglesLineIndex + 1)) % 4

			if moduloIndex == 0:
				currentTexture = line.strip()
				continue

			segments = line.strip().split()

			# Format is: bone vx vy vz nz ny nz u v
			if len(segments) != 9:
				raise RuntimeError(f"{relPath(smdPath)} had invalid vertex definition on line {index + 1}")

			(u, v) = segments[7:9]

			if currentTexture not in self.boundsForTexture:
				self.boundsForTexture[currentTexture] = (u, v, u, v)
			else:
				(minU, minV, maxU, maxV) = self.boundsForTexture[currentTexture]

				if u < minU:
					minU = u

				if v < minV:
					minV = v

				if u > maxU:
					maxU = u

				if v > maxV:
					maxV = v

				self.boundsForTexture[currentTexture] = (minU, minV, maxU, maxV)

	def logUVBounds(self):
		for textureName in self.boundsForTexture:
			(minU, minV, maxU, maxV) = self.boundsForTexture[textureName]
			self.logMsg(f"UV bounds for texture {textureName}: ({minU}, {minV}) - ({maxU}, {maxV})", file="LogUVBounds")

	def createFakeTextures(self, textures:list, outputDir:str):
		global TextureLookup

		if not os.path.isdir(outputDir):
			os.makedirs(outputDir, exist_ok=True)

		for texture in textures:
			textureFileNameOnDisk = TextureLookup[os.path.splitext(texture)[0].lower() + ".png"]
			textureFileNameOnDisk = os.path.join(INPUT_TEXTURE_DIR, textureFileNameOnDisk)

			# We must create fake textures of the appropriate size, otherwise StudioMDL
			# seems to get the UVs wrong...
			width = 0
			height = 0

			with Image.open(textureFileNameOnDisk) as origImage:
				width = origImage.width
				height = origImage.height

			dest = os.path.join(outputDir, texture)
			self.logMsg("Creating fake texture:", relPath(dest), f"({width}x{height})", file="CreateFakeTextures")

			with Image.new("P", (width, height)) as dummyImage:
				dummyImage.save(dest)

	def compileQc(self, qcPath:str):
		baseName = os.path.basename(qcPath)
		workingDir = os.path.dirname(qcPath)
		currentDir = os.curdir

		os.chdir(workingDir)

		args = \
		[
			STUDIOMDL_PATH,
			baseName
		]

		self.runCommand(args, output="CompileQC")

		os.chdir(currentDir)

	def patchMdl(self, mdlPath:str):
		mdlData = None

		with open(mdlPath, "rb") as inFile:
			mdlData = bytearray(inFile.read())

		self.logMsg("Patching", relPath(mdlPath), file="PatchMDL")

		self.patchMdlHeader(mdlPath, mdlData)
		self.patchMdlTexturePaths(mdlPath, mdlData)
		self.patchMdlTextureUVs(mdlPath, mdlData)

		self.logMsg("Patching complete.", file="PatchMDL")

		with open(mdlPath, "wb") as outFile:
			outFile.write(mdlData)

	def patchMdlHeader(self, mdlPath:str, mdlData:bytearray):
		self.logMsg("Adding NO_EMBEDDED_TEXTURES flag to header", file="PatchMDL")

		headerData = struct.unpack_from(MDL_HEADER_FORMAT, mdlData)
		flags = headerData[19] | STUDIO_NO_EMBEDDED_TEXTURES
		struct.pack_into(MDL_HEADER_FORMAT, mdlData, 0, *(headerData[:19]), flags, *(headerData[20:]))

	def patchMdlTexturePaths(self, mdlPath:str, mdlData:bytearray):
		global TextureLookup

		headerData = struct.unpack_from(MDL_HEADER_FORMAT, mdlData)
		numTextures = headerData[30]
		texturesOffset = headerData[31]

		self.logMsg("MDL has", numTextures, "textures beginning at offset", texturesOffset, file="PatchMDL")

		for textureIndex in range(0, numTextures):
			fileOffset = texturesOffset + (textureIndex * MDL_TEXTURE_STRUCT_SIZE)
			textureData = struct.unpack_from(MDL_TEXTURE_FORMAT, mdlData, fileOffset)

			textureName = textureData[0].decode("utf-8").rstrip("\x00")
			textureDirName = os.path.dirname(textureName)
			textureBaseName = os.path.basename(textureName)
			textureNameNoExt = os.path.splitext(textureBaseName)[0]

			# Get the correctly capitalised name of the file on disk, with .png extension
			textureFileNameOnDisk = TextureLookup[textureNameNoExt.lower() + ".png"]

			# Use forward slashes for MDL texture paths
			newTextureName = os.path.join(textureDirName, textureFileNameOnDisk).replace(os.path.sep, "/")

			self.logMsg("Patching", textureName, "->", newTextureName, file="PatchMDL")
			struct.pack_into(MDL_TEXTURE_FORMAT, mdlData, fileOffset, bytes(newTextureName, "utf-8"), *(textureData[1:]))

	def patchMdlTextureUVs(self, mdlPath:str, mdlData:bytearray):
		pass

	def copyPatchedMdl(self, source:str, dest:str):
		self.logMsg("Copying", relPath(source), "to", relPath(dest), file="CopyPatchedMDL")
		shutil.copy2(source, dest)

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
		if texFile.lower() in TextureLookup:
			print("Duplicate texture", texFile, "with different filename case found in textures directory!", file=sys.stderr)
			sys.exit(1)

		TextureLookup[texFile.lower()] = texFile

def cleanScratchDir():
	if os.path.isfile(SCRATCH_DIR):
		os.remove(SCRATCH_DIR)
	elif os.path.isdir(SCRATCH_DIR):
		shutil.rmtree(SCRATCH_DIR)

	os.makedirs(SCRATCH_DIR, exist_ok=True)

def threadTask(files:dict, base:int, stride:int):
	keys = list(files.keys())

	for index in range(base, len(keys), stride):
		filePath = keys[index]
		files[filePath] = False

		try:
			print("Thread", base, "processing file:", relPath(filePath))
			FileProcessor(filePath).processInputFile()
			files[filePath] = True
		except Exception as ex:
			print(str(ex), file=sys.stderr)
			print("*** An error occurred, skipping file", relPath(filePath), file=sys.stderr)
		finally:
			# REMOVE ME
			break

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

	filesMap = {filePath: False for filePath in filesToProcess}
	threadAttributes = [(filesMap, index, NUM_THREADS) for index in range(0, NUM_THREADS)]

	print("Starting thread pool with", NUM_THREADS, "threads")

	with ThreadPoolExecutor(max_workers=NUM_THREADS) as executor:
		executor.map(lambda args: threadTask(*args), threadAttributes)

	filesProcessed = 0
	for filePath in filesMap:
		if filesMap[filePath]:
			filesProcessed += 1

	print("Processed", filesProcessed, "of", len(filesToProcess), "files.")

if __name__ == "__main__":
	main()
