Afterburner Engine
==================

Customised Xash3D FWGS engine for use with [Afterburner](https://github.com/x6herbius/afterburner-game).

For information on the Xash3D FWGS engine, see the [readme](https://github.com/FWGS/xash3d-fwgs/tree/readme).

## Prerequisites

Note that the Afterburner engine does not require the Xash3D VGUI support library to be built, as at the time of writing there are some input issues on Mac when using this library. The game build is set up to use Xash3D's own implementation instead.

### Windows (Visual Studio)
* Install Visual Studio.
* Install latest [Python](https://python.org) **OR** run `cinst python.install` if you have Chocolatey.
* Install latest [Git](https://git-scm.com/download/win) **OR** run `cinst git.install` if you have Chocolatey.
* Download [SDL2](https://libsdl.org/download-2.0.php) development package for Visual Studio.
* Make sure you have at least 12GB of free space to store all build-time dependencies: approx 10GB for Visual Studio, 300 MB for Git, 100 MB for Python and other.

### Linux/Mac
Make sure you're OK with targeting 32-bit. The Xash3D FWGS engine does support targeting 64-bit, but functionality is not guaranteed at this point in time, so 64-bit builds are currently not supported by Afterburner.
**TODO:** See if we can get 64-bit working as the default, Xash3D support seems to be getting better.

#### Debian/Ubuntu
* Enable i386 on your system, if you're compiling 32-bit engine on amd64. If not, skip this.

`$ sudo dpkg --add-architecture i386`
* Install development tools
  * For 64-bit engine on amd64: 
		
    `$ sudo apt install build-essential gcc-multilib g++-multilib python libsdl2-dev:i386 libfontconfig-dev:i386 libfreetype6-dev:i386`
  * For everything else: 
		
    `$ sudo apt install build-essential python libsdl2-dev libfontconfig-dev libfreetype6-dev`

## Building

On any platform, run the following command to clone all required code for the engine. **Make sure** to include the `--recursive` option, otherwise you'll be missing nested repositories.

```
git clone --recursive https://github.com/x6herbius/afterburner-engine
```

Then navigate to the `afterburner-engine` directory in a command prompt, and follow the instructions for your platform:

### Windows (Visual Studio)
1. Examine which build options are available: `waf --help`
2. Configure build: `waf configure --disable-vgui --build-type=release --sdl2=c:/path/to/SDL2 --prefix=c:/path/to/any/output/directory`
3. Compile: `waf build`
4. Install: `waf install`

### Linux/Mac
1. Examine which build options are available: `./waf --help`
2. Configure build: `./waf configure --disable-vgui --build-type=release --win-style-install --prefix=/path/to/any/output/directory`
   Note: Provide the path to SDL2 using the `--sdl2` option if required. On Mac, this is the path up to and including the `SDL2.framework` package.
3. Compile: `./waf build`
4. Install (optional): `./waf install`

# Running
1. Copy libraries and main executable somewhere, if you skipped the installation stage.
2. Copy game files to same directory.
3. Run `afterburner.exe`/`afterburner.sh`/`afterburner` depending on which platform you're using.

For additional info, run Afterburner with `-help` command line key.
