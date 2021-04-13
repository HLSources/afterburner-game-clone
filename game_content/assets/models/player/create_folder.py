import argparse
import os

def __parseArgs():
	parser = argparse.ArgumentParser(description="Helper script for setting up folders for player models.")
	parser.add_argument("name", help="Name of model.")

	return parser.parse_args()

def __qcContents(name):
	return \
f"""// QC file for: {name}
$modelname {name}.mdl
$cd "..\\common_anims"
$cdtexture .
$cliptotextures

$scale 1.0
$origin 0 0 36

// whole body
$bodygroup body
""" + "{\n" + \
f"""studio \"..\\{name}\\{name}_ref\"
""" + "}\n" + \
"""$include "..\common_anims\player_shared.qc"
"""

def __batContents(name):
	return f"""@echo off
..\\..\\..\\..\\..\\utils\\binaries\\afterburner-compile-tools\\abstudiomdl.exe "{name}.qc"
pause"""

def __create(name):
	if os.path.isdir(name):
		raise OSError(f"Directory {name} already exists.")

	os.mkdir(name)

	with open(os.path.join(name, "compile.bat"), "w") as outFile:
		outFile.write(__batContents(name))

	with open(os.path.join(name, f"{name}.qc"), "w") as outFile:
		outFile.write(__qcContents(name))

def __main():
	args = __parseArgs()
	__create(args.name)

if __name__ == "__main__":
	__main()
