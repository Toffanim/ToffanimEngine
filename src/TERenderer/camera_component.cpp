/* ========================================================================
    $File: TERenderer\camera_component.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 02-02-2017 11h57m31s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#include "camera_component.h"

namespace TE
{
	namespace Renderer
	{
		camera_component::camera_component()
		{}

		camera_component::~camera_component()
		{}

		mat4f camera_component::GetView()
		{
			if (_Owner)
			{
				Math::vec3f Position = _Owner->GetPosition();
				Position += _LocalPosition;
				return glm::lookAt(Position, Position + _Axis.Front, _Axis.Up);
			}
			return glm::lookAt(_LocalPosition, _LocalPosition + _Axis.Front, _Axis.Up);
		}

		Math::mat4f camera_component::GetProjection()
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

		void camera_component::SetProjectionType(projection_type Type)
		{
			_Properties.ProjectionType = Type;
		}

		void camera_component::SetCameraProperty(camera_properties Properties)
		{
			_Properties = Properties;
		}
	}
}