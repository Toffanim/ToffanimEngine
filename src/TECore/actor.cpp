#include "actor.h"

namespace TE
{
	namespace Core
	{
		actor::actor() : _Root(new component())
		{
		}

		actor::actor(vec3f Position) : _Position(Position), _Root(new component())
		{
		}

		actor::~actor()
		{
		}

		vec3f actor::GetPosition()
		{
			return (_Position);
		}

		void actor::SetPosition( vec3f NewPosition)
		{
			_Position = NewPosition;
		}

		void actor::AddVector(vec3f Vector)
		{
			_Position += Vector;
		}

		void actor::ExtendBoundingBox(bounding_box NewItemBoundingBox)
		{
			_BoundingBox.LowerBound.x = (NewItemBoundingBox.LowerBound.x < _BoundingBox.LowerBound.x) ? NewItemBoundingBox.LowerBound.x : _BoundingBox.LowerBound.x;
			_BoundingBox.LowerBound.y = (NewItemBoundingBox.LowerBound.y < _BoundingBox.LowerBound.y) ? NewItemBoundingBox.LowerBound.y : _BoundingBox.LowerBound.y;
			_BoundingBox.LowerBound.z = (NewItemBoundingBox.LowerBound.z < _BoundingBox.LowerBound.z) ? NewItemBoundingBox.LowerBound.z : _BoundingBox.LowerBound.z;

			_BoundingBox.UpperBound.x = (NewItemBoundingBox.UpperBound.x > _BoundingBox.UpperBound.x) ? NewItemBoundingBox.UpperBound.x : _BoundingBox.UpperBound.x;
			_BoundingBox.UpperBound.y = (NewItemBoundingBox.UpperBound.y > _BoundingBox.UpperBound.y) ? NewItemBoundingBox.UpperBound.y : _BoundingBox.UpperBound.y;
			_BoundingBox.UpperBound.z = (NewItemBoundingBox.UpperBound.z > _BoundingBox.UpperBound.z) ? NewItemBoundingBox.UpperBound.z : _BoundingBox.UpperBound.z;
		}
	}
}
