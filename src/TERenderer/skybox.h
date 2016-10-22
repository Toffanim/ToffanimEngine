#if !defined(SKYBOX_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Marc TOFFANIN $
   $Notice: Given as-is without warranty AT ALL that it's working$
   ======================================================================== */

#define SKYBOX_H
#include <vector>
#include "TECore\math.h"
#include "GL/glew.h"

#include <iostream>

#include "TERenderer\shader.h"
#include "TECore\utils.h"
#include "TECore\vertex_array.h"

namespace TE
{
	namespace Renderer
	{
		class skybox
		{
		public:
			skybox(const std::vector< const GLchar*>& Faces);
			~skybox();
			void display(Math::mat4f View, Math::mat4f Proj);
		private:
			GLuint loadCubemap(const std::vector<const GLchar*>& faces);
			Core::vertex_array* _VAO;
			GLuint _VBO;
			GLuint _TextureID;
			shader* _Shader;
		};
	}
}

#endif
