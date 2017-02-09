// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\input_component.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m23s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(INPUT_COMPONENT_H)
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

