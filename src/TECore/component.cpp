#include "component.h"
#include <stdlib.h>
#include <iostream>
namespace TE
{
	namespace Core
	{
		component::component()
		{};
		component::~component()
		{
		};

		void component::SetupAttachement(component* Parent)
		{
			_Parent = Parent;
		}
	}
}