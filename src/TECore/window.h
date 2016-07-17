#if !defined(WINDOW_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */
#define WINDOW_H
//NOTE(Marc) : Always call glew before any openGL context creation (GLFW, SDL, etc...)
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <iostream>
#include "../utils/Utils.h"
#include "input_handler.h"
namespace TE
{
	namespace Core
	{
		class window
		{
		public:
			window(int Width, int Height, const char* Title = "Default");

			bool IsResizable() const 
			{ 
				return (_IsResizable); 
			}
			void SetResizable( bool B );

			void SetFullScreen();
			void SetWindowed();

			void SwapBuffers() const;
			void MakeCurrent() const;

			void SetVerticalSync( bool B );
			bool IsVerticalSyncOn() const 
			{
				return (_IsVerticalSyncOn);
			}

			GLFWwindow* GetHandle() const
			{
				return(_WindowHandle);
			}

			int GetWidth() const
			{
				return(_Width);
			}

			int GetHeight() const
			{
				return(_Height);
			}

			void BindInputHandler() const
			{				
				glfwSetKeyCallback(_WindowHandle, &TE::Core::KeyboardCallback);
				glfwSetCursorPosCallback(_WindowHandle, &TE::Core::MouseCursorCallback);
				glfwSetMouseButtonCallback(_WindowHandle, &TE::Core::MouseCallback);
			}

			~window();

		private:
			int _Width;
			int _Height;
			bool _IsResizable;
			bool _IsVerticalSyncOn;
			GLFWwindow* _WindowHandle;
		};
	}
}
#endif

