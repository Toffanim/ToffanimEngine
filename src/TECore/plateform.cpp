#include "plateform.h"

#ifdef _TE_WIN32_
#include "plateform_win32.cpp"
#endif

#ifdef __EMSCRIPTEN__
#include "plateform_web.cpp"
#endif

#ifdef __ANDROID__
#include "plateform_android.cpp"
#endif