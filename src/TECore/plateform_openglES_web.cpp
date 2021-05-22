#include "plateform_openglES_web.h"

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <iostream>

#include "plateform_web.h"

void mainLoop(void* userData) {
	    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        TE::Core::engine* d = (TE::Core::engine*)userData;
       // draw our first triangle
        glUseProgram(d->ShaderProgram);
        glBindVertexArray(d->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
}

namespace TE { namespace Core { namespace Renderer {

void Init(renderer& Renderer) {
    Renderer.Display = Renderer.Plateform->Window.Handle;
// TODO(toffa) : choose the right configuration
	EGLint const attrib_list[] = {
        EGL_RED_SIZE, 4,
        EGL_GREEN_SIZE, 4,
        EGL_BLUE_SIZE, 4,
        EGL_NONE
        };
	
	EGLConfig config;
	EGLint config_size = 1;
	EGLint num_config;
	if( ! eglChooseConfig(Renderer.Display,
			attrib_list,
		    &config,
		    config_size,
		    &num_config)) {
									std::cout << "ERROR: choose config\n" << std::endl;
		return;
			}
		
		
	EGLSurface Surface = eglCreateWindowSurface(	Renderer.Display,
 	                                                config,
 	                                                0,
 	                                                0);
													
	if ( Surface == EGL_NO_SURFACE )
   {
	   std::cout << "ERROR: surface\n" << std::endl;
      return ;
   }
	EGLint const attrib_list_context[] = {
		EGL_CONTEXT_MAJOR_VERSION, 2,
		EGL_NONE, EGL_NONE
	};


    EGLContext Context = eglCreateContext(	Renderer.Display,
 	                                        config,
 	                                        EGL_NO_CONTEXT,
 	                                        attrib_list_context);
											
	if ( Context == EGL_NO_CONTEXT )
   {
	   std::cout << "ERROR: cojtext\n" << std::endl;
      return ;
   }   
											

    if( !eglMakeCurrent(	Renderer.Display,
 	                    Surface,
						Surface,
						Context)) {
							std::cout << "ERROR: make current\n" << std::endl;
						return ;
						}
	Renderer.Surface = Surface;
	Renderer.Context = Context;
}
						

		
}
}
}

