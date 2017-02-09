// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\vertex_array.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m23s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#include "vertex_array.h"

namespace TE
{
	namespace Core
	{
		vertex_array::vertex_array(std::vector<Core::vertex> Vertices, std::vector<unsigned int> Indices) :
			_VertexBuffer(Vertices), _VerticesCount(Vertices.size()), _IndicesCount( Indices.size() )
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

		vertex_array::vertex_array(std::vector<Core::vertex> Vertices) :
			_VertexBuffer(Vertices), _VerticesCount(Vertices.size()), _IndicesCount(0)
		{
			glGenVertexArrays(1, &_ID);
			glBindVertexArray(_ID);

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
			if (_IndicesCount)
			    glDrawElements(GL_TRIANGLES, _IndicesCount, GL_UNSIGNED_INT, (void*)0);
			else
				glDrawArrays(GL_TRIANGLES, 0, _VerticesCount);
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

