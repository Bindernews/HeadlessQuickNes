@echo off
rem Script to build Headless Quick NES using MSVC.
rem Copyright (c) 2016 Drew Heintz. See copyright notice in LICENSE.txt
rem 
rem Open a "Visual Studio Command Prompt"
rem with the appropriate build environment.
rem

if not defined INCLUDE goto :FAIL

setlocal

set SRCDIR=..\src
set HQCOMPILE=cl /nologo /c /O2 /W3 /D_CRT_SECURE_NO_WARNINGS /wd4800 /wd4804 /MD
set HQLINK=link /nologo
set OUTDIR=..\bin
set TARGET=%OUTDIR%\hqnes.dll
set SRC_HQN=..\src\hqn.cpp ^
            ..\src\hqn_gui_controller.cpp ^
            ..\src\hqn_lua.cpp ^
            ..\src\hqn_lua_emu.cpp ^
            ..\src\hqn_lua_gui.cpp ^
            ..\src\hqn_lua_input.cpp ^
            ..\src\hqn_lua_joypad.cpp ^
            ..\src\hqn_lua_mainmemory.cpp ^
            ..\src\hqn_lua_savestate.cpp ^
            ..\src\hqn_surface.cpp ^
            ..\src\hqn_util.cpp

set SRC_QUICKNES=..\quicknes\nes_emu\*.cpp ..\quicknes\fex\*.cpp
set LIB_QUICKNES=..\quicknes\quicknes.lib

if "%1"=="clean" (
    echo Removing all binaries
    del %OUTDIR%\hqnes.dll %OUTDIR%\hqnes.lib %OUTDIR%\hqnes.pdb %OUTDIR%\hqnes.exp %LIB_QUICKNES%
    del *.obj
    goto END
)

rem Check for missing libraries

if not exist ..\LuaJIT (
    echo "Folder LuaJIT not found."
    goto END
)
if not exist ..\SDL2 (
    echo "Folder SDL2 not found."
    goto END
)

rem Enable debug options
if "%DEBUG%"=="1" (
    echo "DEBUG mode enabled"
    set HQCOMPILE=%HQCOMPILE% /MDd /Zi /Od
    set HQLINK=%HQLINK% /DEBUG
)

rem Compile quicknes into a library we can link to.
rem We don't want to compile it multiple times.
if not exist ..\quicknes\quicknes.lib (
    %HQCOMPILE% /I"..\quicknes" /D__LIBRETRO__ /wd4244 /wd4996 %SRC_QUICKNES%
    lib /OUT:%LIB_QUICKNES% *.obj
    del *.obj
)

mkdir %OUTDIR%

rem Compile HQN as a dll
%HQCOMPILE% /DHQNES_SHARED=1 /I"..\quicknes" /I"..\LuaJIT\src" /I"..\SDL2\include" %SRC_HQN%
%HQLINK% /DLL /out:%TARGET% /LIBPATH:"..\SDL2\lib\x86" *.obj "..\LuaJIT\src\lua51.lib" ^
    SDL2.lib SDL2main.lib SDL2_ttf.lib "%LIB_QUICKNES%"
COPY /Y ..\hqnes.lua ..\bin\hqnes.lua
DEL *.obj
:END