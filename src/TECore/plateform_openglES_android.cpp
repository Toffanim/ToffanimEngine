#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "android_native_app_glue.h"
#include <jni.h>
#include <android/log.h>
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
	
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))


struct renderer {
	struct android_app* app;

	EGLDisplay display = EGL_NO_DISPLAY;
	EGLSurface surface = EGL_NO_SURFACE;
	EGLContext context = EGL_NO_CONTEXT;
	
	GLuint shaderProgram;
	GLuint VAO;
};

void init( renderer* engine ) {
    EGLDisplay Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (Display == EGL_NO_DISPLAY) {
		LOGW("Unable to eglGetDisplkay");
      return;
    }

    EGLint Major, Minor;
    if (!eglInitialize(Display, &Minor, &Major)) {
		LOGW("Unable to eglInititalize");
      return;
    }

    // TODO(toffa) : choose the right configuration
    EGLint const attrib_list[] = {EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_RED_SIZE,  8, EGL_GREEN_SIZE, 8,
                                  EGL_BLUE_SIZE, 8, EGL_NONE};

    EGLConfig config;
    EGLint config_size = 1;
    EGLint num_config;
    if (!eglChooseConfig(Display, attrib_list, &config, config_size,
                         &num_config)) {
							 LOGW("Unable to eglChooseConfig");
      return;
    }

    EGLint format;	
	eglGetConfigAttrib(Display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    EGLSurface Surface = eglCreateWindowSurface(Display, config, engine->app->window, 0);

    if (Surface == EGL_NO_SURFACE) {
		LOGW("Unable to eglCreateWindowSurface");
      return;
    }

    EGLint const attrib_list_context[] = {EGL_CONTEXT_CLIENT_VERSION, 3,
                                          EGL_NONE, EGL_NONE};

    EGLContext Context =
        eglCreateContext(Display, config, 0, attrib_list_context);

    if (Context == EGL_NO_CONTEXT) {
		LOGW("Unable to eglCreateContext");
      return;
    }

    if (!eglMakeCurrent(Display, Surface, Surface, Context)) {
		LOGW("Unable to eglMakeCurrent");
      return;
    }
	
	engine->display = Display;
	engine->surface = Surface;
	engine->context = Context;
	
	
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
	LOGW( "ERROR::SHADER::VERTEX::COMPILATION_FAILED %s\n", infoLog);
  }
  // fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
	LOGW( "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s\n", infoLog);
  }
  // link shaders
  unsigned int shaderProgram = glCreateProgram();
  engine->shaderProgram = shaderProgram;
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
	LOGW( "ERROR::SHADER::LINKER::COMPILATION_FAILED %s\n", infoLog);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first Triangle
      1, 2, 3  // second Triangle
  };
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);
  engine->VAO = VAO;

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw( renderer* engine ) {
	if (engine->display == NULL) {
		// No display.
		return;
	}

	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
			// draw our first triangle
    glUseProgram(engine->shaderProgram);
    glBindVertexArray(
        engine->VAO); // seeing as we only have a single VAO there's no need to bind it
              // every time, but we'll do so to keep things a bit more organized
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	eglSwapBuffers(engine->display, engine->surface);
}

void handle_cmd(android_app *pApp, int32_t cmd) {
renderer* engine = (renderer*) pApp->userData;	
  switch (cmd) {
  case APP_CMD_INIT_WINDOW: {
   init(engine);
  } break;

  case APP_CMD_TERM_WINDOW:
    // TODO :: toffa: clenaup
    break;
  }
}

/**
 * Process the next input event.
 */
static int32_t handle_inputs(struct android_app *app, AInputEvent *event) {
  return 0;
}



void android_main(struct android_app *pApp) {
	renderer engine;
	
	pApp->userData = &engine;
  pApp->onAppCmd = handle_cmd;
  pApp->onInputEvent = handle_inputs;

engine.app = pApp;  

LOGW("START NATIVEAPP APPLICATINON");

  int events;
  android_poll_source *pSource;
  do {
    if (ALooper_pollAll(0, nullptr, &events, (void **)&pSource) >= 0) {
      if (pSource) {
        pSource->process(pApp, pSource);
      }
    }
    draw(&engine);
  } while (!pApp->destroyRequested);
}
