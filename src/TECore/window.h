/* ========================================================================
    $File: TECore\window.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 02-02-2017 11h57m31s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(WINDOW_H)
#define WINDOW_H
//NOTE(Marc) : Always call glew before any openGL context creation (GLFW, SDL, etc...)
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "input_handler.h"


/*! \file window.h
\brief Window creation and management

*/

namespace TE
{
	namespace Core
	{
		/*!
		window is the class used to create a window with an opengl context.
		The window has severals properties : resizable, fullscreen, windowed, etc

		IMPORTANT : For now you can only create one window per application, because we can't correctly create multiple opengl contexts.
		To avoid this problem we should switch to GLFW 3.2 and GLEW MX
		*/
		class window
		{
		public:
			/*!
			window ctor 
			\param Width Desired window width
			\param Height Desired window height
			\param Title Desired window title, defaulted as "Default"
			*/
			window(int Width, int Height, std::string Title = "Default");

			static void ResizeCallback(GLFWwindow* Window, int width, int height)
			{
				auto _Window = static_cast<window*>(glfwGetWindowUserPointer(Window));
				_Window->SetWidth( width );
				_Window->SetHeight( height );
			}

			static void CloseCallback(GLFWwindow* Window)
			{
				auto _Window = static_cast<window*>(glfwGetWindowUserPointer(Window));
				_Window->Close();
			}

			void Close()
			{
				_IsRunning = false;
			}

			bool Running()
			{
				return(_IsRunning);
			}

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


			void SetWidth(int Width)
			{
				_Width = Width;
			}

			void SetHeight(int Height)
			{
				_Height = Height;
			}

			int GetWidth() const
			{
				return(_Width);
			}

			int GetHeight() const
			{
				return(_Height);
			}

			void HideCursor() const;
			void ShowCursor() const;

			void BindInputHandler() const
			{				
				glfwSetKeyCallback(_WindowHandle, &TE::Core::KeyboardCallback);
				glfwSetCursorPosCallback(_WindowHandle, &TE::Core::MouseCursorCallback);
				glfwSetMouseButtonCallback(_WindowHandle, &TE::Core::MouseCallback);
			}

			~window();

		private:
			std::string _Title;
			int _Width;
			int _Height;
			bool _IsResizable;
			bool _IsVerticalSyncOn;
			bool _IsFullscreen;
			bool _IsRunning;
			GLFWwindow* _WindowHandle;
		};
	}
}
#endif

