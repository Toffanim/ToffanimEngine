#include "window.h"

namespace TE
{
	namespace Core
	{
		window::window(int Width, int Height, const char* Title)
			:_Width(Width),
			_Height(Height)
		{
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
			_WindowHandle = glfwCreateWindow(Width, Height, Title, NULL, NULL);
			if (!_WindowHandle)
			{
				std::cerr << "ERROR : Window creation failed" << std::endl;
			}
			glfwSetInputMode(_WindowHandle, GLFW_STICKY_KEYS, GL_TRUE);
			glfwSetInputMode(_WindowHandle, GLFW_STICKY_MOUSE_BUTTONS, 1);
		}


		window::~window()
		{
			glfwDestroyWindow(_WindowHandle);
		}

		void window::MakeCurrent()
		{
			//We have to make the context current before initializing Glew
			glfwMakeContextCurrent(_WindowHandle);
			if (glewInit() != GLEW_OK)
			{
				std::cout << "Failed to initialize GLEW" << std::endl;
			}
			Utils::checkGlError("GLEW INIT (Safe To Ignore)");
		}

		void window::SetVerticalSync(bool B)
		{
			_IsVerticalSyncOn = B;
			if (B)
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);
		}

		void window::SwapBuffers()
		{
			glfwSwapBuffers(_WindowHandle);
			glfwPollEvents();
		}
	}
}
