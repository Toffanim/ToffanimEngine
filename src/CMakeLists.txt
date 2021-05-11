cmake_minimum_required(VERSION 3.6.0)
project(MainNativeActivity LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -fno-exceptions -fno-rtti")

add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(${PROJECT_NAME}
  SHARED
    main.cpp
)
set_target_properties(${PROJECT_NAME}
  PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
set_target_properties(${PROJECT_NAME}
  PROPERTIES  LINK_FLAGS "-u ANativeActivity_onCreate")

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    native_app_glue EGL GLESv3
)
