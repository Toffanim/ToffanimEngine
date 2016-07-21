#include "sprite.h"

namespace TE
{
	namespace Renderer
	{		
		sprite::sprite(Core::texture2D& Texture) : _Texture(Texture)
		{
			//Geometry
			int   quad_triangleCount = 2;
			int   quad_triangleList[] = { 0, 1, 2, 2, 1, 3 };
			float quad_vertices[] = { -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0 };
			// Quad
			GLuint vao;
			glGenVertexArrays(1, &vao);
			GLuint vbo[2];
			glGenBuffers(2, vbo);
			glBindVertexArray(vao);
			// Bind indices and upload data
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_triangleList), quad_triangleList, GL_STATIC_DRAW);
			// Bind vertices and upload data
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (void*)0);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
		}

		sprite::sprite(const std::string FilePath)
			: _Texture(Core::texture2D(FilePath))
		{
		}

		sprite::~sprite()
		{
		}

		void sprite::Render()
		{
			
			TE::GBufferFBO->Bind();
		}
	}
}

