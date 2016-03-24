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

@if not defined DEBUG goto :COMPILE
@set HQCOMPILE=%HQCOMPILE% /MDd /Zi /Od
@set HQLINK=%HQLINK% /DEBUG

@rem Compile quicknes
:COMPILE
MKDIR %OUTDIR%
%HQCOMPILE% /I"..\quicknes" /wd4244 /wd4996 %SRC_QUICKNES%
%HQCOMPILE% /I"..\quicknes" /I"..\LuaJIT\src" %SRC_HQN%
%HQLINK% /out:%HQEXENAME% *.obj "..\LuaJIT\src\lua51.lib"
DEL *.obj