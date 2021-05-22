#pragma once

#include <dsound.h>

#include "window.h"
#include "plateform_opengl_win32.h"

#define TE_MAIN() void main() { \
        Plateform::plateform CurrentPlateform = {}; \
        Plateform::Init(CurrentPlateform); \
        te_main_entry(CurrentPlateform); \
    } \
    void te_main_entry( Plateform::plateform& CurrentPlateform )

namespace TE { 
namespace Core { 

namespace Window {
    struct window {
        HWND Handle;
    };
} // namespace Window
namespace Audio {
   struct buffer {
       LPDIRECTSOUNDBUFFER Handle;
   };
   struct device {
      LPDIRECTSOUND8 Handle; 
      buffer PrimaryBuffer;
   };
} // namespace Audio
namespace Plateform {
  struct plateform {
    Window::window Window;
    Audio::device AudioDevice;
  };

  namespace Files {
      struct file {
          HANDLE Handle;
      };
  } // namespace Files
} // namespace Plateform
} // namespace Core
} // namespace T