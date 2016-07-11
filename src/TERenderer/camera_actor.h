#if !defined(CAMERA_ACTOR_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define CAMERA_ACTOR_H

#include "../TECore/actor.h"

namespace TE
{
	namespace Renderer
	{
		class camera_actor : public Core::actor
		{
		public:
			camera_actor();
			camera_actor(vec3f Position);
			~camera_actor();

			void BeginTick() override {};
			void Tick() override {};

			Math::mat4f GetView();

		private:
			//Math::axis3 _Axis;
			//Math::rotation _Rotation;
		};
	}
}

#endif