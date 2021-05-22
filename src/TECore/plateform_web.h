#pragma once

#include <EGL/egl.h>
#include "window.h"
#include <stdio.h>
#include <emscripten/val.h>
using namespace emscripten;

#define TE_MAIN() int main() { \
        Plateform::plateform CurrentPlateform = {}; \
        Plateform::Init(CurrentPlateform); \
        te_main_entry(CurrentPlateform); \
    } \
    void te_main_entry( Plateform::plateform& CurrentPlateform )

namespace TE {
namespace Core {
namespace Window {

struct window {
    EGLDisplay Handle;
};

} // namespace Window

namespace Audio {
   struct buffer {
       val Handle = val::object();
       val Context = val::object();
       val Source = val::object();
   };
   struct device {
       val AudioContext = val::object();
       val Context = val::object();
   };
} // namespace Audio

namespace Plateform {

    struct plateform {
        Audio::device AudioDevice;
        Window::window Window;
    };

  namespace Files {
      struct file {
          FILE* Handle;
      };
  } // namespace Files

} // namespace Plateform 
} // namespace Core
} // namespace TE
