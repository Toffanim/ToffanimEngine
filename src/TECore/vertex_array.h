#if !defined(VERTEX_ARRAY_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

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

