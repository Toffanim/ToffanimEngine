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
#include "glew/glew.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <iostream>

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
			void SetResizable( const bool B );

			void SetFullScreen();
			void SetWindowed();

			void SwapBuffers();
			void MakeCurrent();

			void SetVerticalSync( const bool B );
			bool IsVerticalSyncOn() const 
			{
				return (_IsVerticalSyncOn);
			}

			void SetKeyCallback();
			void SetMouseCallback();
			void SetCursorCallback();

			GLFWwindow* GetHandle()
			{
				return(_WindowHandle);
			}

			int GetWidth()
			{
				return(_Width);
			}

			int GetHeight()
			{
				return(_Height);
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

