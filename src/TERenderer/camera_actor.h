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
#include <stdlib.h>
#include <iostream>

namespace TE
{
	namespace Renderer
	{
		enum projection_type
		{
			PERSPECTIVE,
			ORTHOGRAPHIC,
		};

		struct camera_properties
		{
			projection_type ProjectionType = PERSPECTIVE;
			float MinDist = 0.1f;
			float MaxDist = 100.f;
			float Zoom = 45.f;
			float AspectRatio = 1.f;
		};

		class camera_actor : public Core::actor
		{
		public:
			camera_actor();
			camera_actor(vec3f Position);
			~camera_actor();

			void BeginTick() override {};
			void Tick( float DeltaTime ) override {};

			void SetCameraProperty(camera_properties Properties);
			Math::mat4f GetView();
			Math::mat4f GetProjection();

			void HandleCursor(double xpos, double ypos);

			void SetProjectionType(projection_type Type);

		private:
			camera_properties _Properties;
			float _LastX;
			float _LastY;
			bool _IsFirstTime;
		};
	}
}

#endif