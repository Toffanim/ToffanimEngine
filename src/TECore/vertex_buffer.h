/* ========================================================================
    $File: TECore\vertex_buffer.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 02-02-2017 11h57m31s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(VERTEX_BUFFER_H)
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

