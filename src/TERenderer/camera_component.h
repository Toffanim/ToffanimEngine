#if !defined(CAMERA_COMPONENT_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define CAMERA_COMPONENT_H

#include "TERenderer\camera_actor.h"
#include "TECore\component.h"
#include "TECore\math.h"

namespace TE
{
	namespace Renderer
	{
		class camera_component : public Core::component_CRTP < camera_component >
		{
		public:
			camera_component();
			~camera_component();

			void Tick(float DeltaTime) override {};
			void Render(Math::mat4f Projection, Math::mat4f View) override {};

			void SetCameraProperty(camera_properties Properties);
			Math::mat4f GetView();
			Math::mat4f GetProjection();

			void SetProjectionType(projection_type Type);

			void SetLocalPosition(Math::vec3f LocalPosition)
			{
				_LocalPosition = LocalPosition;
			}

		private:
			camera_properties _Properties;
			Math::vec3f _LocalPosition;
			Math::axis3 _Axis;

		};
	}
}

#endif