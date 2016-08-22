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
#include <stdlib.h>
#include <vector>

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

			virtual ~actor();

			//Called the first time the actor is actually rendered in the scene
			virtual void BeginTick() {};
            //Called every frame
			virtual void Tick(float DeltaTime) 
			{  
				for (auto Component : _Childs)
					Component->Tick(DeltaTime);
			};

			virtual void Render( Math::mat4f Projection, Math::mat4f View)
			{
				for (auto Component : _Childs)
					Component->Render(Projection, View);
			}

			vec3f GetPosition();
			vec3f GetLocalPosition();

			void SetPosition( vec3f NewPosition );
			void SetLocalPosition(vec3f NewPosition );

			void AddVector(vec3f Vector);
			void AddLocalVector(vec3f Vector);
			void AddRotation(vec3f Rotation);
			void AddRotation(double Yaw, double Pitch, double Roll);
			void AddScale(vec3f Scale);
			void Transform(vec3f Translation,
				vec3f Rotation,
				vec3f Scale);

			component* GetRoot() const {
				return(_Root);
			};

			bounding_box GetAABB()
			{
				return(_BoundingBox);
			}

			Math::axis3 GetAxis() const
			{
				return(_Axis);
			}

			void ExtendBoundingBox(bounding_box NewItemBoundingBox);
		
		protected:			
			void UpdateAxis();

			component* _Root;
			std::vector< component* > _Childs;
			vec3f _Position;
			rotation _Rotation;
			axis3 _Axis;
			mat4f _ModelTransform;
			bounding_box _BoundingBox;

		};
	}
}
#endif