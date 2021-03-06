// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\vertex_array.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m23s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(VERTEX_ARRAY_H)
#define VERTEX_ARRAY_H

#include <GL/glew.h>
#include <stdlib.h>
#include <vector>
#include "TECore\vertex.h"
#include "TECore\vertex_buffer.h"

namespace TE
{
	namespace Core
	{
		class vertex_array
		{
		public:
			vertex_array( std::vector<Core::vertex> Vertices, std::vector<unsigned int> Indices);
			vertex_array(std::vector<Core::vertex> Vertices);
			~vertex_array();

			void Bind() const;
			void Render() const;
			void Update(std::vector<Core::vertex> NewVertices);

		private:
			GLuint _ID;
			Core::vertex_buffer _VertexBuffer;
			GLuint _EBO;
			size_t _IndicesCount;
			size_t _VerticesCount;
		};
	}
}


#endif

