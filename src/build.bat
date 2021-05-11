@echo off

ctime -begin TE.ctm

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

IF %1 == WIN64 (

IF NOT EXIST WIN64 mkdir WIN64
pushd WIN64

del *.pdb > NUL 2> NUL

cl ..\..\src\main.cpp -D_TE_WIN32_ /W4 /nologo /link /SUBSYSTEM:console -incremental:no gdi32.lib opengl32.lib user32.lib kernel32.lib

popd
goto EOF
)

IF %1 == WEB (

IF NOT EXIST WEB mkdir WEB
pushd WEB

em++ ..\..\src\main.cpp -o main.html --emrun
emrun main.html

popd
goto EOF
)

IF %1 == ANDROID (

IF NOT EXIST ANDROID mkdir ANDROID
pushd ANDROID

cmake -DANDROID_STL=c++_static -DCMAKE_TOOLCHAIN_FILE=C:/Microsoft/AndroidNDK64/android-ndk-r16b/build/cmake/android.toolchain.cmake -DANDROID_PLATFORM=android-19 -DANDROID_ABI=x86_64 ../../src 

popd
goto EOF
)

:EOF
popd

ctime -end TE.ctm %LastError%