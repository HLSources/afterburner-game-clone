#!/bin/bash

cd $GITHUB_WORKSPACE

if [ "$GH_CPU_ARCH" == "i386" ]; then
	sudo dpkg --add-architecture i386
	sudo apt update
	sudo apt install gcc-multilib g++-multilib libx11-dev:i386 libxext-dev:i386 x11-utils libgl1-mesa-dev libasound-dev libstdc++6:i386 libfuse2:i386 zlib1g:i386

	wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-i686.AppImage" -O appimagetool.AppImage
elif [ "$GH_CPU_ARCH" == "amd64" ]; then
	sudo apt update
	sudo apt install gcc-multilib g++-multilib libx11-dev libxext-dev x11-utils libgl1-mesa-dev libasound-dev libstdc++6 libfuse2 zlib1g

	wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage" -O appimagetool.AppImage
else
	exit 1
fi

chmod +x appimagetool.AppImage

wget http://libsdl.org/release/SDL2-$SDL_VERSION.zip -O SDL2.zip
unzip -q SDL2.zip
mv SDL2-$SDL_VERSION SDL2_src
