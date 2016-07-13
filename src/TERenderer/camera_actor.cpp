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
