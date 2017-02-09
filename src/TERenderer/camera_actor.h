// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TERenderer\camera_actor.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m24s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(CAMERA_ACTOR_H)
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
