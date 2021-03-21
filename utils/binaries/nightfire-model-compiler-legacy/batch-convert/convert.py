import sys
import os
import subprocess
import shutil
import struct

SCRIPT_DIR = os.path.dirname(os.path.realpath(sys.argv[0]))
INPUT_DIR = os.path.join(SCRIPT_DIR, "v14")
INPUT_TEXTURE_DIR = os.path.join(INPUT_DIR, "textures")
OUTPUT_DIR = os.path.join(SCRIPT_DIR, "v10")
SCRATCH_DIR = os.path.join(SCRIPT_DIR, "scratch")
TEXTURE_DIR_NAME = "mdl"
SCRATCH_TEXTURE_DIR = os.path.join(SCRATCH_DIR, TEXTURE_DIR_NAME)

MDLCONVERT_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "mdlconvert.exe"))
STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "studiomdl_new.exe"))
BLACK_BMP_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "black.bmp"))

MDL_HEADER_FORMAT = "II64sIfffffffffffffffIIIIIIIIIIIIIIIIIIIIIIIIIII"
MDL_TEXTURE_FORMAT = "64sIIII"
MDL_TEXTURE_STRUCT_SIZE = struct.calcsize(MDL_TEXTURE_FORMAT)
STUDIO_NO_EMBEDDED_TEXTURES = 0x800

# Map from all-lowercase texture names to actual names
TextureLookup = {}

def relPath(path:str):
	return os.path.relpath(path, SCRIPT_DIR)

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

	if not os.path.isfile(BLACK_BMP_PATH):
		print("Black.bmp file", BLACK_BMP_PATH, "does not exist.", file=sys.stderr)
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

def runCommand(args):
	print("Running command:", *args)

	result = subprocess.run(args, shell=True)

	if result.returncode != 0:
		raise RuntimeError(f"Command {' '.join(args)} returned error code {result.returncode}")

	print("Command complete.")

def dumpToQc(inputFile:str, outputDir:str):
	fileName = os.path.splitext(os.path.basename(inputFile))[0]
	outputPath = os.path.join(outputDir, f"{fileName}.qc")

	args = \
	[
		MDLCONVERT_PATH,
		"-dumpv14tov10",
		inputFile,
		outputPath
	]

	runCommand(args)
	return outputPath

def fixSmdTextures(smdPath:str):
	global TextureLookup

	print("Fixing referenced textures in SMD:", relPath(smdPath))

	texturesToCopy = {}
	lines = []
	begunTriangles = False

	with open(smdPath, "r") as inFile:
		lines = [line.rstrip() for line in inFile.readlines()]

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

		if textureName not in texturesToCopy:
			print("Texture referenced:", textureName)

			# Record that we encountered this texture.
			texturesToCopy[textureName] = True

		# Rename each bmp file to an appropriate png, namespaced in an "mdl" folder
		lines[index] = f"{TEXTURE_DIR_NAME}/{textureName}"

	with open(smdPath, "w") as outFile:
		# The final \n is required or studiomdl freaks out
		outFile.write("\n".join(lines) + "\n")

	# Return all the textures we need to copy.
	return texturesToCopy

def fixSmdFiles(dirPath:str):
	texturesToCopy = {}

	for smdFile in os.listdir(dirPath):
		ext = os.path.splitext(smdFile)[1].lower()

		if ext != ".smd":
			continue

		texturesFromFile = fixSmdTextures(os.path.abspath(os.path.join(dirPath, smdFile)))

		for key in texturesFromFile.keys():
			texturesToCopy[key] = True

	return list(texturesToCopy.keys())

def createFakeTextures(textures:list, outputDir:str):
	global TextureLookup

	if not os.path.isdir(outputDir):
		os.makedirs(outputDir, exist_ok=True)

	for texture in textures:
		dest = os.path.join(outputDir, texture)
		print("Creating fake texture:", relPath(dest))
		shutil.copy2(BLACK_BMP_PATH, dest)

def compileQc(qcPath:str):
	baseName = os.path.basename(qcPath)
	workingDir = os.path.dirname(qcPath)
	currentDir = os.curdir

	os.chdir(workingDir)

	args = \
	[
		STUDIOMDL_PATH,
		baseName
	]

	runCommand(args)

	os.chdir(currentDir)

def patchMdlTexturePaths(mdlPath:str):
	global TextureLookup

	print("Patching textures in:", relPath(mdlPath))

	mdlData = None

	with open(mdlPath, "rb") as inFile:
		mdlData = bytearray(inFile.read())

	headerData = struct.unpack_from(MDL_HEADER_FORMAT, mdlData)
	numTextures = headerData[30]
	texturesOffset = headerData[31]

	print(relPath(mdlPath), "has", numTextures, "textures beginning at offset", texturesOffset)

	print("Adding NO_EMBEDDED_TEXTURES flag to header")
	flags = headerData[19] | STUDIO_NO_EMBEDDED_TEXTURES
	struct.pack_into(MDL_HEADER_FORMAT, mdlData, 0, *(headerData[:19]), flags, *(headerData[20:]))

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

		print("Patching", textureName, "->", newTextureName)
		struct.pack_into(MDL_TEXTURE_FORMAT, mdlData, fileOffset, bytes(newTextureName, "utf-8"), *(textureData[1:]))

	with open(mdlPath, "wb") as outFile:
		outFile.write(mdlData)

	print(relPath(mdlPath), "patched.")

def copyPatchedMdl(source:str, dest:str):
	print("Copying", relPath(source), "to", relPath(dest))
	shutil.copy2(source, dest)

def processInputFile(filePath:str):
	# Make sure the scratch dir exists and is empty.
	cleanScratchDir()

	# Dump the selected model to the scratch dir.
	qcPath = dumpToQc(filePath, SCRATCH_DIR)

	# Fix all texture paths in all SMDs.
	textures = fixSmdFiles(SCRATCH_DIR)

	# Create fake textures somewhere that StudioMDL can see them.
	createFakeTextures(textures, SCRATCH_TEXTURE_DIR)

	# Compile the QC file.
	compileQc(qcPath)

	# Patch the texture paths so that they point to PNGs.
	mdlPath = os.path.splitext(qcPath)[0] + ".mdl"
	patchMdlTexturePaths(mdlPath)

	# Copy the patched model to the target directory.
	inputMdlRelPath = os.path.relpath(filePath, INPUT_DIR)
	copyPatchedMdl(mdlPath, os.path.join(OUTPUT_DIR, inputMdlRelPath))

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

	filesToProcess = getInputFiles(INPUT_DIR)
	print("Found", len(filesToProcess), "input files")

	numProcessed = 0

	for filePath in filesToProcess:
		try:
			print("Processing file:", relPath(filePath))
			processInputFile(filePath)
			numProcessed += 1
		except Exception as ex:
			print(str(ex), file=sys.stderr)
			print("*** An error occurred, skipping file", relPath(filePath), file=sys.stderr)
		finally:
			# REMOVE ME
			if numProcessed >= 3:
				break

	print("Processed", numProcessed, "of", len(filesToProcess), "files.")

if __name__ == "__main__":
	main()
