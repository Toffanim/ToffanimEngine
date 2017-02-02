/* ========================================================================
    $File: TECore\actor.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 02-02-2017 11h57m31s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#include "actor.h"

namespace TE
{
	namespace Core
	{
		actor::actor() : _Root(new default_component()),
			_Childs(std::vector<component*>({ _Root }))
		{
			_Root->SetupAttachement(this);
		}

		actor::actor(vec3f Position) : _Position(Position), _Root(new default_component()),
			_Childs(std::vector<component*>({ _Root }))
		{
			_Root->SetupAttachement(this);
		}

		actor::~actor()
		{
			for (auto Child : _Childs)
			{
				if (Child)
					delete Child;
			}
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

		void actor::AddLocalVector(vec3f Vector)
		{   
			auto AllAxis = Vector.x * _Axis.Right + Vector.y * _Axis.Front + Vector.z * _Axis.Up;
			_Position += AllAxis;
		}

		void actor::AddRotation(vec3f Rotation)
		{
			float Tmp = fmod(_Rotation.Roll + Rotation.x, 360);
			_Rotation.Roll = Tmp;
			Tmp = fmod(_Rotation.Pitch + Rotation.y, 360);
			_Rotation.Pitch = Tmp;
			Tmp = fmod(_Rotation.Yaw + Rotation.z, 360);
			_Rotation.Yaw = Tmp;
			UpdateAxis();
		}

		void actor::AddRotation(double Roll, double Pitch, double Yaw)
		{
			float Tmp = fmod(_Rotation.Roll + Roll, 360);
			_Rotation.Roll = Tmp;
			Tmp = fmod(_Rotation.Pitch + Pitch, 360);
			_Rotation.Pitch = Tmp;
			Tmp = fmod(_Rotation.Yaw + Yaw, 360);
			_Rotation.Yaw = Tmp;
			UpdateAxis();
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

		void actor::UpdateAxis()
		{
			axis3 DefaultAxis;
			Math::quaternion Quat = Math::RotationAsQuaternion(_Rotation);
			Math::mat3f Rotation = Quat.AsRotationMatrix();
			_Axis.Front = DefaultAxis.Front * Rotation;
			_Axis.Right = DefaultAxis.Right * Rotation;
			_Axis.Up = DefaultAxis.Up * Rotation;
		}
	}
}
