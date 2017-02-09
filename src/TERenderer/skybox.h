// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TERenderer\skybox.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m25s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(SKYBOX_H)
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
