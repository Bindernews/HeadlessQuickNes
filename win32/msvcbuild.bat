@rem Script to build Headless Quick NES using MSVC.
@rem Copyright (c) 2016 Drew Heintz. See copyright notice in LICENSE.txt
@rem 
@rem Open a "Visual Studio Command Prompt"
@rem with the appropriate build environment.
@rem

@if not defined INCLUDE goto :FAIL

@setlocal
@set HQCOMPILE=cl /nologo /c /O2 /W3 /D_CRT_SECURE_NO_WARNINGS /wd4800 /wd4804 /MD
@set HQLINK=link /nologo
@set HQMT=mt /nologo
@set OUTDIR=..\bin
@set HQEXENAME=%OUTDIR%\hqnes.exe
@set SRC_HQN=..\src\*.cpp
@set SRC_QUICKNES=..\quicknes\nes_emu\*.cpp ..\quicknes\fex\*.cpp

@rem Check for missing libraries
@if not exist ..\LuaJIT (
    @echo "Folder LuaJIT not found."
    @goto END
)
@if not exist ..\SDL2 (
    @echo "Folder SDL2 not found."
    @goto END
)

@if not defined DEBUG goto :COMPILE
@set HQCOMPILE=%HQCOMPILE% /MDd /Zi /Od
@set HQLINK=%HQLINK% /DEBUG

:COMPILE
MKDIR %OUTDIR%
@rem Compile quicknes
%HQCOMPILE% /I"..\quicknes" /wd4244 /wd4996 %SRC_QUICKNES%
@rem Compile HQN
%HQCOMPILE% /I"..\quicknes" /I"..\LuaJIT\src" /I"..\SDL2\include" %SRC_HQN%
%HQLINK% /out:%HQEXENAME% *.obj "..\LuaJIT\src\lua51.lib" "..\SDL2\lib\x86\SDL2.lib" "..\SDL2\lib\x86\SDL2main.lib"
DEL *.obj
:END