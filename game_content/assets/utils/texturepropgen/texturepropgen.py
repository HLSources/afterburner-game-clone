import argparse
import sys
import os

def parseArguments():
	parser = argparse.ArgumentParser(description="Creates texture property files in bulk.")

	parser.add_argument("-d", "--directory",
						required=True,
						help="Directory within which to search for files.")

	return parser.parse_args()

def createPropertiesFile(path, surfacePropName):
	print("Creating", path, "for surface prop", surfacePropName)

	with open(path, "w") as outFile:
		outFile.write(f"surfaceprop {surfacePropName}")

def createPropertiesInDirectory(directory, surfacePropName):
	print("Creating", surfacePropName, "property files for contents in", directory)

	for item in os.listdir(directory):
		fullPath = os.path.join(directory, item)

		if os.path.isdir(fullPath):
			createPropertiesInDirectory(fullPath, surfacePropName)
		elif os.path.isfile(fullPath) and os.path.splitext(item)[1] == ".png":
			createPropertiesFile(os.path.join(directory, os.path.splitext(item)[0] + ".props"), surfacePropName)

def main():
	args = parseArguments()

	if not os.path.isdir(args.directory):
		print("The directory", args.directory, "was not valid.")

	print("Creating property files for all items recursively in", args.directory)
	for item in os.listdir(args.directory):
		fullPath = os.path.join(args.directory, item)

		if os.path.isdir(fullPath):
			createPropertiesInDirectory(fullPath, item)

if __name__ == "__main__":
	main()
