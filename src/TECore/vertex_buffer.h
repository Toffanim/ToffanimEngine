#if !defined(VERTEX_BUFFER_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define VERTEX_BUFFER_H

#include "GL/glew.h"
#include <stdlib.h>
#include <vector>
#include "TECore\vertex.h"

namespace TE
{
	namespace Core
	{
		class vertex_buffer
		{
		public:
			vertex_buffer( std::vector<Core::vertex> Vertices);
			~vertex_buffer();

			void AddData();
			void Bind();
			void Update(std::vector<Core::vertex> NewVertices);

		private:
			GLuint _ID;
		};
	}
}


#endif

