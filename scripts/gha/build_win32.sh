#!/bin/bash

. scripts/lib.sh

# Build engine
cd $BUILDDIR

if [ "$ARCH" = "amd64" ]; then # we need enabling 64-bit target only on Intel-compatible CPUs
	AMD64="-8"
fi

# NOTE: to build with other version use --msvc_version during configuration
# NOTE: sometimes you may need to add WinSDK to %PATH%
./waf.bat configure -s "SDL2_VC" -T "debug" --prefix=`pwd` --enable-utils $AMD64 || die
./waf.bat build -v || die
./waf.bat install || die

if [ "$ARCH" = "i386" ]; then
	cp SDL2_VC/lib/x86/SDL2.dll . # Install SDL2
	cp vgui-dev/lib/win32_vc6/vgui.dll .
elif [ "$ARCH" = "amd64" ]; then
	cp SDL2_VC/lib/x64/SDL2.dll .
else
	die
fi

7z a -t7z $BUILDDIR/xash3d-fwgs-win32-$ARCH.7z -m0=lzma2 -mx=9 -mfb=64 -md=32m -ms=on *.dll *.exe *.pdb

echo "Generating VC2008 project"
rm -rf vc2008/
mkdir vc2008/
./waf.bat msdev
cp *.sln vc2008/
find . -name "*.vcproj" -exec cp --parents \{\} vc2008/ \;
rm -rf vc2008/vc2008 # HACKHACK

7z a -t7z $BUILDDIR/xash3d-fwgs-vc2008-sln-$ARCH.7z -m0=lzma2 -mx=9 -mfb=64 -md=32m -ms=on -r vc2008
