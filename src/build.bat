@echo off

ctime -begin TE.ctm

set CURRENT_DIR=%~dp0

IF NOT EXIST %CURRENT_DIR%..\build mkdir %CURRENT_DIR%..\build
pushd %CURRENT_DIR%..\build

IF %1 == WIN64 (

IF NOT EXIST WIN64 mkdir WIN64
pushd WIN64

del *.pdb > NUL 2> NUL

call cl ..\..\src\main.cpp -D_TE_WIN32_ /W4 /nologo /link /SUBSYSTEM:console -incremental:no gdi32.lib opengl32.lib user32.lib kernel32.lib dsound.lib

popd
goto end
)

IF %1 == WEB (

IF NOT EXIST WEB mkdir WEB
pushd WEB
IF NOT EXIST assets mkdir assets

call em++ ..\..\src\main.cpp -o main.html -g --bind --preload-file assets@/ --emrun -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2
call emrun main.html

popd
goto end
)

IF %1 == ANDROID (

IF NOT EXIST ANDROID mkdir ANDROID
pushd ANDROID

call rm -rf *

IF NOT EXIST jni mkdir jni
IF NOT EXIST lib mkdir lib\x86_64
IF NOT EXIST res mkdir res
IF NOT EXIST assets mkdir assets

set ANDROID_SDK=%CURRENT_DIR%..\lib\android_sdk\
set ANDROID_TOOLS=%ANDROID_SDK%tools\
set ANDROID_BUILD_TOOLs=%ANDROID_SDK%build-tools\31.0.0-rc3\
set ANDROID_PLATEFORM_TOOLS=%ANDROID_SDK%platform-tools\
set ANDROID_CMDLINE_TOOLS=%ANDROID_SDK%cmdline-tools\
set ANDROID_PLATEFORM=%ANDROID_SDK%platforms\
set ANDROID_EMULATOR=%ANDROID_SDK%emulator\

set ANDROID_NDK=%ANDROID_SDK%ndk-bundle\
set ANDROID_NDK_TOOLCHAIN=%ANDROID_NDK%toolchains\

set CC=%ANDROID_NDK_TOOLCHAIN%llvm\prebuilt\windows-x86_64\bin\clang
set CXX=%ANDROID_NDK_TOOLCHAIN%llvm\prebuilt\windows-x86_64\bin\clang++
set AR=%ANDROID_NDK_TOOLCHAIN%llvm\prebuilt\windows-x86_64\bin\x86_64-linux-android-ar

set CXX_INCLUDES=%ANDROID_NDK%sources\android\native_app_glue
set CXX_FLAGS=-D __ANDROID__ -D ANDROID --target=x86_64-linux-android26

REM Create native_glue static library
call %CC% %CXX_FLAGS% -c %ANDROID_NDK%sources/android/native_app_glue/android_native_app_glue.c -o jni/native_app_glue.o
call %AR% rcs jni/libnative_app_glue.a jni/native_app_glue.o
REM Create native library from main
call %CXX% %CXX_FLAGS% -Wall -I %CXX_INCLUDES% -std=gnu++11 -fPIC -c %CURRENT_DIR%main.cpp -o jni/main.o
call %CXX% %CXX_FLAGS% -I %CXX_INCLUDES% -u ANativeActivity_onCreate -L jni -shared jni/main.o -o lib/x86_64/libMainNativeActivity.so -llog -landroid -lEGL -lGLESv3  -lnative_app_glue -lOpenSLES
REM Create Java DEX file for JNI interface
REM call javac -classpath %ANDROID_PLATEFORM%android-26/android.jar %CURRENT_DIR%AndroidFileSystem.java
REM call %ANDROID_BUILD_TOOLS%dx --dex --output=. %CURRENT_DIR% 
REM Create unsigned and unaligned apk
call %ANDROID_BUILD_TOOLS%aapt2 link -I %ANDROID_PLATEFORM%android-26\android.jar -o MainApp-unaligned-unsigned.apk --manifest %CURRENT_DIR%AndroidManifest.xml
call echo F|xcopy /R /Y /F %ANDROID_NDK_TOOLCHAIN%llvm\prebuilt\windows-x86_64\sysroot\usr\lib\x86_64-linux-android\libc++_shared.so lib\x86_64\libc++_shared.so
call echo F|xcopy /R /Y /F %CURRENT_DIR%test.wav assets\test.wav
call %ANDROID_BUILD_TOOLS%aapt add MainApp-unaligned-unsigned.apk lib/x86_64/libc++_shared.so
call %ANDROID_BUILD_TOOLS%aapt add MainApp-unaligned-unsigned.apk lib/x86_64/libMainNativeActivity.so
call %ANDROID_BUILD_TOOLS%aapt add MainApp-unaligned-unsigned.apk assets/test.wav
REM call %ANDROID_BUILD_TOOLS%aapt add MainApp-unaligned-unsigned.apk classes.dex
REM Align the apk
call %ANDROID_BUILD_TOOLS%zipalign -v -p 4 MainApp-unaligned-unsigned.apk MainApp-unsigned.apk
REM Sign the apk
call %ANDROID_BUILD_TOOLS%apksigner sign --ks %CURRENT_DIR%ToyKey.keystore --ks-pass pass:armena --out MainApp.apk MainApp-unsigned.apk
REM Start Emulator
REM %ANDROID_EMULATOR%emulator -avd testAVD
REM Deploy application
call %ANDROID_PLATEFORM_TOOLS%adb uninstall com.MainApp
call %ANDROID_PLATEFORM_TOOLS%adb install MainApp.apk

REM sdkmanager --install build-tools plateform-tools plateform-android25 ndk-bundle emulator system-images;android-25 extra;intel;hardware_acceleraition....
REM avdmanager create avd --force --name testAVD --abi google_apis/x86_64 --package system-images;android-25;google_apis;x86_64
REM "C:\Program Files\Android\jdk\microsoft_dist_openjdk_1.8.0.25\bin\keytool.exe" -genkeypair -validity 1000 -dname "CN=some company,O=Android,C=JPN" -keystore ToyKey.keystore         -storepass armena -keypass armena -alias NativeActivityKey -keyalg RSA

popd
goto end
)

:end
popd

set LastError = %ERRORLEVEL%

ctime -end TE.ctm %LastError%