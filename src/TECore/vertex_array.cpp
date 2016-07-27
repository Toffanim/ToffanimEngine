#include "vertex_array.h"

namespace TE
{
	namespace Core
	{
		vertex_array::vertex_array(std::vector<Core::vertex> Vertices, std::vector<unsigned int> Indices) :
			_VertexBuffer(Vertices), _IndicesCount( Indices.size() )
		{
			glGenVertexArrays(1, &_ID);
			glBindVertexArray(_ID);

			// Element array buffer for triangles indices
			glGenBuffers(1, &_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), &Indices[0], GL_STATIC_DRAW);

			// Enable vertex pointer
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Core::vertex), (GLvoid*)0);
			// Vertex Normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Core::vertex), (GLvoid*)offsetof(Core::vertex, Normal));
			// Vertex Texture Coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Core::vertex), (GLvoid*)offsetof(Core::vertex, TextureCoords));
		}

		void vertex_array::Bind() const
		{
			glBindVertexArray(_ID);
		}

		void vertex_array::Render() const
		{
			Bind();
			glDrawElements(GL_TRIANGLES, _IndicesCount, GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(0);
		}

		void vertex_array::Update(std::vector<Core::vertex> NewVertices)
		{
			_VertexBuffer.Update(NewVertices);
		}

		vertex_array::~vertex_array()
		{
			glDeleteVertexArrays(1, &_ID);
		}
	}
}

