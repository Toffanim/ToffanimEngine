// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\vertex_buffer.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m23s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#include "vertex_buffer.h"

namespace TE
{
	namespace Core
	{
		vertex_buffer::vertex_buffer(std::vector<Core::vertex> Vertices)
		{
			glGenBuffers(1, &_ID);
			glBindBuffer(GL_ARRAY_BUFFER, _ID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Core::vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
		}

		void vertex_buffer::Bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, _ID);
		}

		void vertex_buffer::Update(std::vector<Core::vertex> NewVertices)
		{
			//NOTE: Test if NEwVertices.size() != Vertices.soze();
			Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(Core::vertex) * NewVertices.size(), NULL, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Core::vertex) * NewVertices.size(), &NewVertices[0], GL_STATIC_DRAW);
		}

		vertex_buffer::~vertex_buffer()
		{
			glDeleteBuffers(1, &_ID);
		}
	}
}

