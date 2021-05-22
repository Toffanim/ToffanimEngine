#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "plateform_android.h"
	
namespace TE { 
namespace Core { 
namespace Renderer {

struct renderer {
	Plateform::plateform* Plateform;

	EGLDisplay Display = EGL_NO_DISPLAY;
	EGLSurface Surface = EGL_NO_SURFACE;
	EGLContext Context = EGL_NO_CONTEXT;
};

void Init(renderer& Renderer) {
    if (Renderer.Plateform == nullptr) return;

    Renderer.Display = Renderer.Plateform->Window.Handle;
    Renderer.Plateform->App->userData = &Renderer;
    
    // Force handle_cmd init window
    // because it will create the context needed for further OpenGL calls
    while(!gIsWindowAvailable) {
        int events;
        android_poll_source *pSource;
        if (ALooper_pollAll(0, nullptr, &events, (void **)&pSource) >= 0) {
          if (pSource) {
            pSource->process(Renderer.Plateform->App, pSource);
          }
        }
    }
}

// NOTe(toffa): needed because the init will actually happen
// inside the handl_cmd function when WINDOW is init by device
// We cannot do it before. This is kinda a hack to make it work with android_glue
void InitEGL(renderer& Renderer) {
    // TODO(toffa) : choose the right configuration
    EGLint const attrib_list[] = {EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_RED_SIZE,  8, EGL_GREEN_SIZE, 8,
                                  EGL_BLUE_SIZE, 8, EGL_NONE};

    EGLConfig config;
    EGLint config_size = 1;
    EGLint num_config;
    if (!eglChooseConfig(Renderer.Display, attrib_list, &config, config_size,
                         &num_config)) {
							 LOGW("Unable to eglChooseConfig");
      return;
    }

    EGLint format;	
	  eglGetConfigAttrib(Renderer.Display, config, EGL_NATIVE_VISUAL_ID, &format);

  	ANativeWindow_setBuffersGeometry(Renderer.Plateform->App->window, 0, 0, format);

    EGLSurface Surface = eglCreateWindowSurface(Renderer.Display, config, Renderer.Plateform->App->window, 0);

    if (Surface == EGL_NO_SURFACE) {
		LOGW("Unable to eglCreateWindowSurface");
      return;
    }

    EGLint const attrib_list_context[] = {EGL_CONTEXT_CLIENT_VERSION, 3,
                                          EGL_NONE, EGL_NONE};

    EGLContext Context =
        eglCreateContext(Renderer.Display, config, 0, attrib_list_context);

    if (Context == EGL_NO_CONTEXT) {
		LOGW("Unable to eglCreateContext");
      return;
    }

    if (!eglMakeCurrent(Renderer.Display, Surface, Surface, Context)) {
		  LOGW("Unable to eglMakeCurrent");
      return;
    }
	
	Renderer.Surface = Surface;
	Renderer.Context = Context;
}

} // namespace Renderer


namespace Window {
void Init( window& Window, const char* Name, const char* Title) {
    // TODO(toffa): can we name the window?
    // Name and Title unused for now on web
   EGLDisplay Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if ( Display == EGL_NO_DISPLAY )
   {
	   LOGW( "ERROR: make display\n" );
      return ;
   }
	
	EGLint Major,Minor;
	if( !eglInitialize(Display, &Minor, &Major) ) {
		LOGW( "ERROR: make init" );
		return ;
	}

  Window.Handle = Display;
}


void Show(window& Window) {
    // TODO(toffa): is it implementable?
   // Not implemented for web as window is always shown? 
}

} // namespace Window
} // namespace Core
}	// namespace TE

