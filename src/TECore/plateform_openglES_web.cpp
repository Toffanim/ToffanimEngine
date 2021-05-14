#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include "../../lib/emsdk/upstream/emscripten/system/include/emscripten/emscripten.h"
#include <iostream>

const char *vertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in mediump vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 300 es\n"
    "out mediump vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
	
	struct data {
		GLuint shader;
		GLuint VAO;
	};
	
void mainLoop(void* userData) {
	    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

data* d = (data*)userData;
       // draw our first triangle
        glUseProgram(d->shader);
        glBindVertexArray(d->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
}

int main() {
	
	std::cout << "HELLO WORLD" << std::endl;
	data d;
	
	EGLDisplay Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	
	if ( Display == EGL_NO_DISPLAY )
   {
	   std::cout << "ERROR: make display\n" << std::endl;
      return EGL_FALSE;
   }
	
	EGLint Major,Minor;
	if( !eglInitialize(Display, &Minor, &Major) ) {
		std::cout << "ERROR: make init\n" << std::endl;
		return 0;
	}
	
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
	if( ! eglChooseConfig(Display,
			attrib_list,
		    &config,
		    config_size,
		    &num_config)) {
									std::cout << "ERROR: choose config\n" << std::endl;
		return -1;
			}
		
		
	EGLSurface Surface = eglCreateWindowSurface(	Display,
 	                                                config,
 	                                                0,
 	                                                0);
													
	if ( Surface == EGL_NO_SURFACE )
   {
	   std::cout << "ERROR: surface\n" << std::endl;
      return EGL_FALSE;
   }
													
	EGLint const attrib_list_context[] = {
		EGL_CONTEXT_MAJOR_VERSION, 2,
		EGL_NONE, EGL_NONE
	};
	
    EGLContext Context = eglCreateContext(	Display,
 	                                        config,
 	                                        EGL_NO_CONTEXT,
 	                                        attrib_list_context);
											
	if ( Context == EGL_NO_CONTEXT )
   {
	   std::cout << "ERROR: cojtext\n" << std::endl;
      return EGL_FALSE;
   }   
											

    if( !eglMakeCurrent(	Display,
 	                    Surface,
						Surface,
						Context)) {
							std::cout << "ERROR: make current\n" << std::endl;
						return -1;
						}
						
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
	d.shader = shaderProgram;
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
	d.VAO = VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    emscripten_set_main_loop_arg( mainLoop, &d, 0, true );
						// TODO(toffa): cleanup sequence
		
	return 0;
}

