#if !defined(CAMERA_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator:  $
   $Notice: $
   ======================================================================== */

#define CAMERA_H

#include <glew/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace TE
{
	namespace Renderer
	{
		class camera
		{
		public:
			camera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f),
				GLfloat Yaw = 90.0f, GLfloat Pitch = 0.0f);

			glm::mat4 GetViewMatrix();

			GLfloat GetZoom() const { return(_Zoom); }

			void SetPosition(glm::vec3 NewPos) { _Position = NewPos; }
			glm::vec3 GetPosition() const { return(_Position); }

			//Possible only in 3D
			void AddYaw(float Value);
			void AddPitch(float Value);
			
			glm::vec3 GetFront(){ return(_Front); }
		private:
			glm::vec3 _Position;
			glm::vec3 _Up;
			GLfloat _Yaw;
			GLfloat _Pitch;
			GLfloat _Zoom;
			glm::vec3 _Front;
			glm::vec3 _Right;

			void UpdateCameraVectors();
		};
	}
}

#endif
