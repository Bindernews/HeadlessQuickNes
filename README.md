# Introduction
HappyQuickNes (HQN) uses the QuickNes emulator to emulate NES games. It also includes Lua scripting support with LuaJIT.
HQN is designed for distributed computing and to that end it includes the Lua sockets library as well.

# Building
Build scripts are provided for Windows and Linux. For other platforms it shouldn't be too hard.

## All
You will need LuaJIT. Make sure to extract it and rename the folder to LuaJIT (remove the version).
The LuaJIT folder should be in the root of the HappyQuickNes folder.

## Linux
The Makefile is `linux/Makefile`. The makefile should automatically build the required libraries.

## Windows
A batch file can be found in `win32\msvcbuild.bat`. This will compile both the quicknes library
and HQN. This leaves the LuaJIT library which can be compiled using `LuaJIT\src\msvcbuild.bat`
(remember to `cd` to the directory before running msvcbuild.bat). This requires the Visual Studio
command prompt.

# Running
## Linux
When HQN is run it will say it is missing a required library. Copy LuaJIT/src/libluajit.so
to the directory where hqnes is and rename it to the name of the required library. This will
usually be something like `libluajit-5.1.so.2`.


## Windows
Copy lua51.dll to the directory where hqnes.exe is located.

# Lua Sockets
## Linux
Run the command `sudo apt-get install luarocks && sudo luarocks install luasocket` or the appropriate
command for your Linux distribution.

## Windows
Download the luasockets distribution from [LuaPower](https://luapower.com/socket).
