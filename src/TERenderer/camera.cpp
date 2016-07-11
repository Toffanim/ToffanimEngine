/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator:  $
   $Notice:  $
   ======================================================================== */
#include "Camera.h"

namespace TE
{
	namespace Renderer
	{
		camera::camera(glm::vec3 Position, glm::vec3 Up, GLfloat Yaw, GLfloat Pitch) : 
			_Position(Position), 
			_Up(Up), 
			_Yaw(Yaw), 
			_Pitch(Pitch)
		{
			_Front = glm::vec3(0.0f, 0.0f, -1.0f);
			_Zoom = 45.0f;
			UpdateCameraVectors();
		}

		glm::mat4 camera::GetViewMatrix()
		{
			return glm::lookAt(_Position, _Position + _Front, _Up);
		}

		void camera::UpdateCameraVectors()
		{
			// Calculate the new Front vector
			glm::vec3 Front;
			Front.x = cos(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
			Front.y = sin(glm::radians(_Pitch));
			Front.z = sin(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
			_Front = glm::normalize(Front);
		}

		void camera::AddYaw(float Value)
		{
			_Yaw += Value;
			UpdateCameraVectors();
		}


		void camera::AddPitch(float Value)
		{
			_Pitch += Value;
			if (_Pitch> 89.0f)
				_Pitch = 89.0f;
			if (_Pitch < -89.0f)
				_Pitch = -89.0f;
			UpdateCameraVectors();
		}
	}
}
