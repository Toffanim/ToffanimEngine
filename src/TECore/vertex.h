/* ========================================================================
    $File: TECore\vertex.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 02-02-2017 11h57m31s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(VERTEX_H)
#define VERTEX_H
#include "math.h"

namespace TE
{
	namespace Core
	{
		struct vertex
		{
			Math::vec3f Position;
			Math::vec3f Normal;
			Math::vec2f TextureCoords;
		};
	}
}


#endif