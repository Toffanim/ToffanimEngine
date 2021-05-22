#pragma once

namespace TE { 
namespace Core { 
namespace Renderer {

struct renderer {
    Plateform::plateform* Plateform;

    EGLDisplay Display = EGL_NO_DISPLAY;
	EGLSurface Surface = EGL_NO_SURFACE;
	EGLContext Context = EGL_NO_CONTEXT;
};

} // namespace Renderer
} // namespace Core
} // namespace TE 