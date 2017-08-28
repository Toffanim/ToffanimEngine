// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\input_handler.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 22-08-2017 01h15m33s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(INPUT_HANDLER_H)
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

		static void KeyboardCallback(GLFWwindow* , int Key, int Scancode, int Action, int Mods)
		{
			TE::DefaultInputHandler->DispatchKey(Key, Action);
		}

		static void MouseCursorCallback(GLFWwindow* , double XPos, double YPos)
		{
			TE::DefaultInputHandler->DispatchMouseCursor(XPos, YPos);
		}

		static void MouseCallback(GLFWwindow*, int Button, int Action, int Mods)
		{
			TE::DefaultInputHandler->DispatchMouseKey(Button, Action);
		}
	}
}
#endif

