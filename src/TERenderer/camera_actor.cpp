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
	}
}
