@echo off

ctime -begin teengine.ctm

set OutputFile=engine.exe
set OutputDir=..\build\Debug\

set ObjOutputDir=..\build\engine.dir\Debug\

set CommonMacros =/D GLEW_STATIC /D WIN32 /D _WINDOWS /D _DEBUG /D _MBCS /D_CRT_SECURE_NO_WARNINGS

set CommonCompilerFlags=/Od /MDd /DEBUG /Oi /W4 /GS /TP /Zc:wchar_t /Zi /Gm- -EHsc /nologo -fp:fast -fp:except- -Fe%OutputDir%%OutputFile% /Fo%ObjOutputDir% %CommonMacros% -wd4505 -wd4201 -wd4100 -wd4996
set CommonLinkerFlags=-incremental:no -opt:ref /ignore:4217 /ignore:4049
set ExternalLibraries=kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib ..\build\lib\glfw\src\Debug\glfw3.lib ..\build\lib\assimp\code\Debug\assimp-vc140-mt.lib ..\build\Debug\imgui.lib ..\build\Debug\stb.lib ..\build\lib\EASTL\Debug\EASTL.lib ..\build\Debug\glew.lib glu32.lib opengl32.lib ..\build\lib\assimp\contrib\zlib\Debug\zlibstaticd.lib
set ExternalLibrariesInclude=/I..\src /I..\lib\glm /I..\lib\imgui /I..\lib\stb /I..\lib\glew\include /I..\lib\glfw\include /I..\lib\imgui\examples\libs\glfw\include -I..\lib\EASTL\include -I..\lib\EASTL\test\packages\EABase\include\Common

set SourcesToCompil= ..\src\main.cpp ..\src\TECore\*.cpp ..\src\TERenderer\*.cpp ..\src\TEPhysics\*.cpp 

cl %CommonCompilerFlags% %ExternalLibrariesInclude% %SourcesToCompil% /link %CommonLinkerFlags% %ExternalLibraries% /IMPLIB:"%OutputDir%engine.lib" /PDB:"../build/Debug/engine.pdb" /MACHINE:X64

set LastError=%ERRORLEVEL%

ctime -end teengine.ctm %LastError%