#if !defined(INPUT_HANDLER_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define INPUT_HANDLER_H

#include "globals.h"
#include "GLFW\glfw3.h"
#include <stdlib.h>
#include <vector>
#include <memory>
#include "input_component.h"


namespace TE
{
	namespace Core
	{
		class input_handler
		{
		public:
			input_handler();
			~input_handler();

			void Add(std::shared_ptr<input_component> c)
			{
				_Controllers.push_back(c);
			}

			void Delete(input_component* c)
			{
				//TODO
			}

			void DispatchKey( int Key, int Action)
			{
				for (auto Controller : _Controllers)
				{
					Controller->ProcessKey(Key, Action);
				}
			}

			void DispatchMouseKey(int Button, int Action)
			{
				for (auto Controller : _Controllers)
				{
					Controller->ProcessMouseKey(Button, Action);
				}
			}

			void DispatchMouseCursor(double XPos, double YPos)
			{
				for (auto Controller : _Controllers)
				{
					Controller->ProcessCursor(XPos, YPos);
				}
			}

		private:
			std::vector<std::shared_ptr<input_component>> _Controllers;
		};

		static void KeyboardCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
		{
			TE::DefaultInputHandler->DispatchKey(Key, Action);
		}

		static void MouseCursorCallback(GLFWwindow* Window, double XPos, double YPos)
		{
			TE::DefaultInputHandler->DispatchMouseCursor(XPos, YPos);
		}

		static void MouseCallback(GLFWwindow* Window, int Button, int Action, int Mods)
		{
			TE::DefaultInputHandler->DispatchMouseKey(Button, Action);
		}
	}
}
#endif

