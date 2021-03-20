import sys
import os
import subprocess
import shutil

SCRIPT_DIR = os.path.dirname(os.path.realpath(sys.argv[0]))
INPUT_DIR = os.path.join(SCRIPT_DIR, "v14")
OUTPUT_DIR = os.path.join(SCRIPT_DIR, "v10")
SCRATCH_DIR = os.path.join(SCRIPT_DIR, "scratch")
TEXTURE_DIR_NAME = "mdl"
SCRATCH_TEXTURE_DIR = os.path.join(SCRATCH_DIR, TEXTURE_DIR_NAME)

MDLCONVERT_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "mdlconvert.exe"))
STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "studiomdl_new.exe"))
BLACK_BMP_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "black.bmp"))

Indent = 0

# REMOVE ME
Processed = False

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

def processInputFile(filePath):
	global Processed

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

	Processed = True

def iterateOverInputFiles(rootDir:str):
	global Processed
	global Indent

	filesProcessed = 0
	totalFiles = 0

	for fileEntry in os.listdir(rootDir):
		fullPath = os.path.abspath(os.path.join(rootDir, fileEntry))

		if os.path.isdir(fullPath):
			(nestedProcessed, nestedTotal) = iterateOverInputFiles(fullPath)

			filesProcessed += nestedProcessed
			totalFiles += nestedTotal
			continue

		fileExt = os.path.splitext(fileEntry)[1].lower()

		if fileExt != ".mdl":
			continue

		try:
			Indent = 1
			print("Processing file:", relPath(fullPath))
			processInputFile(fullPath)
			filesProcessed += 1
		except Exception as ex:
			print(str(ex), file=sys.stderr)
			print("Skipping file", relPath(fullPath), file=sys.stderr)
			Processed = True
		finally:
			Indent = 0
			totalFiles += 1
			if Processed:
				return (filesProcessed, totalFiles)

	return (filesProcessed, totalFiles)

def main():
	validateDirs()
	(processed, total) = iterateOverInputFiles(INPUT_DIR)

	print("Finished processing", processed, "of", total, "files from", INPUT_DIR)

if __name__ == "__main__":
	main()
