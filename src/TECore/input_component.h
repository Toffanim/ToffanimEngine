#if !defined(INPUT_COMPONENT_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define INPUT_COMPONENT_H
#include "component.h"
#include <stdlib.h>
#include <map>
#include <functional>
#include "globals.h"
#include <memory>

namespace TE
{
	namespace Core
	{
		class input_component : public component_CRTP<input_component>, public std::enable_shared_from_this<input_component>
		{
		public:
			input_component();
			~input_component();

			void Init();

			void ProcessKey(int Key, int Action);
			void ProcessMouseKey(int Button, int Action);
			void ProcessCursor(double XPos, double YPos);
			void AddKeyBind(std::pair< int, int > Key_Action, const std::function<void(void)> &Func);
			void AddMouseBind(std::pair< int, int > Button_Action, const std::function<void(void)> &Func);
			void AddCursorBind(const std::function<void(double, double)> &Func);

		private:
			std::map< std::pair<int, int>, std::function<void(void)> > _MappingKey;
			std::map< std::pair<int, int>, std::function<void(void)> > _MappingMouseKey;
			std::function<void(int, int)> _CursorCallback;
		};
	}
}

#endif

