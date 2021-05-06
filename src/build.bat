@echo off

ctime -begin TE.ctm

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

del *.pdb > NUL 2> NUL

cl ..\src\main.cpp -D_TE_WIN32_ /W4 /nologo /link /SUBSYSTEM:console -incremental:no gdi32.lib opengl32.lib user32.lib kernel32.lib

popd

ctime -end TE.ctm %LastError%