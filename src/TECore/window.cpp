#include "window.h"

namespace TE
{
	namespace Core
	{
		window::window(int Width, int Height, std::string Title)
			:_Width(Width),
			_Height(Height),
			_Title(Title)
		{
			// OpenGL context hard constraints
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
			// Window hard constraints
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
			// Window related hints
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
			glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
			glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
			glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
			glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
			glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
			// Framebuffer related hints
			glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
			glfwWindowHint(GLFW_SAMPLES, 0);
			// Context related hints
#ifndef NDEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

			_WindowHandle = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);
			if (!_WindowHandle)
			{
				std::cerr << "ERROR : Window creation failed" << std::endl;
			}
			glfwSetInputMode(_WindowHandle, GLFW_STICKY_KEYS, GLFW_TRUE);
			glfwSetInputMode(_WindowHandle, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
			glfwSetInputMode(_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			glfwSetWindowUserPointer(_WindowHandle, this);

			glfwSetWindowSizeCallback(_WindowHandle, &window::ResizeCallback);
			glfwSetWindowCloseCallback(_WindowHandle, &window::CloseCallback);

			BindInputHandler();

			_IsRunning = true;
			_IsFullscreen = false;
			_IsResizable = true;
		}

		window::~window()
		{
			glfwDestroyWindow(_WindowHandle);
		}

		void window::MakeCurrent() const
		{
			//We have to make the context current before initializing Glew
			glfwMakeContextCurrent(_WindowHandle);
			glewExperimental = true;
			if (glewInit() != GLEW_OK)
			{
				std::cout << "Failed to initialize GLEW" << std::endl;
			}
			Core::CheckOpenGLError("GLEW INIT (Safe To Ignore)");
		}

		void window::SetVerticalSync(bool B)
		{
			_IsVerticalSyncOn = B;
			if (B)
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);
		}


		void window::SetFullScreen()
		{
			glfwSetWindowMonitor(_WindowHandle, glfwGetPrimaryMonitor(), 0, 0, _Width, _Height, GLFW_DONT_CARE);
			_IsFullscreen = true;
		}

		void window::SetWindowed()
		{
			glfwSetWindowMonitor(_WindowHandle, NULL, 0, 0, _Width, _Height, GLFW_DONT_CARE);
			_IsFullscreen = false;
		}

		void window::HideCursor() const
		{
			glfwSetInputMode(_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		void window::ShowCursor() const
		{
			glfwSetInputMode(_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		void window::SwapBuffers() const
		{
			glfwSwapBuffers(_WindowHandle);
			glfwPollEvents();
		}
	}
}
