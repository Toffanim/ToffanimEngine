#if !defined(ACTOR_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define ACTOR_H

#include "component.h"
#include "math.h"

using namespace TE::Math;

namespace TE
{
	namespace Core
	{
		// class that describe any object that can be present in the scene
		// having basic functions to move it and compute a bouding box
		class actor
		{
		public:
			actor();
			actor(vec3f Position);

			~actor();

			//Called the first time the actor is actually rendered in the scene
			virtual void BeginTick() = 0;
            //Called every frame
			virtual void Tick() = 0;

			vec3f GetPosition();
			vec3f GetLocalPosition();

			void SetPosition( vec3f NewPosition );
			void SetLocalPosition(vec3f NewPosition );

			void AddVector(vec3f Vector);
			void AddRotation(vec3f Rotation);
			void AddScale(vec3f Scale);
			void Transform(vec3f Translation,
				vec3f Rotation,
				vec3f Scale);

		protected:
			void ExtendBoundingBox(bounding_box NewItemBoundingBox);

			component* _Child;
			vec3f _Position;
			rotation _Rotation;
			axis3 _Axis;
			mat4f _ModelTransform;
			bounding_box _BoundingBox;

		};
	}
}
#endif