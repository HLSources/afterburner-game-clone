import os
import sys
import argparse
import spritecreation
from PIL import Image

def parseArguments():
	parser = argparse.ArgumentParser(description="Converts images to engine-compatible sprite files.")

	parser.add_argument("-i", "--input",
						required=True,
						help="Input image file.")

	parser.add_argument("-o", "--output",
						required=True,
						help="Output sprite file.")

	return parser.parse_args()

def convertFile(input : str, output : str):
	print("Reading:", input)

	if not os.path.isfile(input):
		raise OSError(f"File {input} was not found.")

	image = Image.open(input)
	print("Image data:", len(image.getdata()), "bytes.")

	sprite = spritecreation.createSingleFrameSprite(image)
	print("Writing:", output)

	with open(output, "wb") as outFile:
		outFile.write(sprite.header)
		outFile.write(sprite.frameType)
		outFile.write(sprite.data)

	print("Written successfully.")

def main():
	args = parseArguments()
	convertFile(args.input, args.output)
	print("Done.")

if __name__ == "__main__":
	main()
