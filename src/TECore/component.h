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

			virtual void Tick(float DeltaTime)
			{}

			virtual void Render()
			{}

			void SetupAttachement(component* Parent);

		private:
			component* _Parent;
			component* _Child;
		};
	}
}
#endif