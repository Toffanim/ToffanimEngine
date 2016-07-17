#if !defined(COMPONENT_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define COMPONENT_H

namespace TE
{
	namespace Core
	{
		class component
		{
		public:
			component();
			~component();

			void SetupAttachement(component* Parent);

		private:
			component* _Parent;
			component* _Child;
		};
	}
}
#endif