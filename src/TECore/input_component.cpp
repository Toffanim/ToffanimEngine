#include "input_component.h"

namespace TE
{
	namespace Core
	{
		//TODO(Marc) : Make this thing more robust, with binding of "Named Action" to a function call
		//This way we can bind whatever button to the "Named Action" and it will work

		// Make it autobind to the default input_handler, circular dependency ? 
		input_component::input_component() : component()
		{
		}


		input_component::~input_component()
		{
		}

		void input_component::Init()
		{
		}

		void input_component::ProcessKey(int Key, int Action)
		{

			const auto & Func = _MappingKey[ {Key, Action} ];
			if (Func)
			{
				Func();
			}
		}

		void input_component::ProcessCursor(double XPos, double YPos)
		{
			if (_CursorCallback)
				_CursorCallback(XPos, YPos);
		}

		void input_component::ProcessMouseKey(int Button, int Action)
		{
			const auto & func = _MappingMouseKey[{Button, Action}];
			if (func)
			{
				func();
			}
		}

		void input_component::AddKeyBind(std::pair< int, int > Key_Action, const std::function<void(void)> &Func)
		{
			_MappingKey[Key_Action] = Func;
		}

		void input_component::AddMouseBind(std::pair< int, int > Button_Action, const std::function<void(void)> &Func)
		{
			_MappingMouseKey[Button_Action] = Func;
		}

		void input_component::AddCursorBind(const std::function<void(double, double)> &Func)
		{
			_CursorCallback = Func;
		}

	}
}
