#!/bin/sh

APP=Xash3DFWGS
ARCH=i686
APPDIR=$APP-i386.AppDir

# Copy all needed files
./waf install
mv appimage $APPDIR

# Generate extras.pak
python3 scripts/makepak.py xash-extras/ $APPDIR/extras.pak

cp SDL2_linux/lib/libSDL2-2.0.so.0 $APPDIR/
cp vgui-dev/lib/vgui.so $APPDIR/

cat > $APPDIR/AppRun << 'EOF'
#!/bin/sh

echo "Xash3D FWGS installed as AppImage."

if [ "$XASH3D_BASEDIR" = "" ]; then
	export XASH3D_BASEDIR=$PWD
fi
export XASH3D_EXTRAS_PAK1="${APPDIR}"/extras.pak
export LD_LIBRARY_PATH="${APPDIR}":$LD_LIBRARY_PATH
${DEBUGGER} "${APPDIR}"/xash3d "$@"
exit $?
EOF

chmod +x $APPDIR/xash3d # Engine launcher
chmod +x $APPDIR/AppRun # Engine launcher script

echo "Contents of AppImage: "
ls $APPDIR

wget "https://raw.githubusercontent.com/FWGS/fwgs-artwork/master/xash3d/icon_512.png" -O $APPDIR/$APP.png

cat > $APPDIR/$APP.desktop <<EOF
[Desktop Entry]
Name=$APP
Icon=$APP
Type=Application
Exec=AppRun
Categories=Game;
EOF

wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-$ARCH.AppImage"
chmod +x appimagetool-$ARCH.AppImage
./appimagetool-$ARCH.AppImage $APPDIR 
