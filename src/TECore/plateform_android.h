#pragma once

#include "plateform_openglES_android.h"
#include "android_native_app_glue.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

#define TE_MAIN() void android_main(struct android_app *pApp) { \
        Plateform::plateform CurrentPlateform = {}; \
        CurrentPlateform.App = pApp; \
        Plateform::Init(CurrentPlateform); \
        te_main_entry(CurrentPlateform); \
    } \
    void te_main_entry( Plateform::plateform& CurrentPlateform )

namespace TE { 
namespace Core { 

namespace Audio {
   struct buffer {
     size_t size;
     char* data;
     SLObjectItf playerObject;
   };
   struct device {
     SLEngineItf Handle;
     SLObjectItf outputMixObject;
   };
} // namespace Audi

namespace Plateform {

  struct plateform {
    struct android_app* App;

    Audio::device AudioDevice;
    Window::window Window;
  };
  namespace Files {
      struct file {
          int Handle;
          AAsset* Asset;
      };
  } // namespace Files
} // namespace Plateform

} // namespace Core
} // namespace TE
  