// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TERenderer\camera_actor.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m24s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#include "camera_actor.h"

namespace TE
{
	namespace Renderer
	{
		camera_actor::camera_actor() : actor()
		{
		}

		camera_actor::camera_actor(vec3f Position) : actor(Position)
		{
		}

		camera_actor::~camera_actor()
		{
		}

		void camera_actor::HandleCursor(double xpos, double ypos)
		{
			if (_IsFirstTime)
			{
				_LastX = xpos;
				_LastY = ypos;
				_IsFirstTime = false;
			}

			int xoff = xpos - _LastX;
			int yoff = ypos - _LastY;
			_LastX = xpos;
			_LastY = ypos;

			float sensitivity = 0.5f;
			xoff *= sensitivity;
			yoff *= -sensitivity;

			if (_Rotation.Roll + yoff > 89)
			{
				yoff = 89 - _Rotation.Roll;
			}

			if (_Rotation.Roll + yoff < -89)
			{
				yoff = -89 - _Rotation.Roll;
			}

			AddRotation(yoff,0, -xoff);
		}

		mat4f camera_actor::GetView()
		{
			return glm::lookAt(_Position, _Position + _Axis.Front , _Axis.Up);
		}

		Math::mat4f camera_actor::GetProjection()
		{
			if (_Properties.ProjectionType == projection_type::PERSPECTIVE)
			{
				return glm::perspective(_Properties.Zoom, _Properties.AspectRatio,
					_Properties.MinDist, _Properties.MaxDist);
			}

			if (_Properties.ProjectionType == projection_type::ORTHOGRAPHIC)
			{
				return glm::ortho(50.f, 50.f, 50.f, 50.f, _Properties.MinDist, _Properties.MaxDist);
			}

			std::cerr << "Projection type is unknown" << std::endl;
			return Math::mat4f();
		}

		void camera_actor::SetProjectionType(projection_type Type)
		{
			_Properties.ProjectionType = Type;
		}

		void camera_actor::SetCameraProperty(camera_properties Properties)
		{
			_Properties = Properties;
		}
	}
}
