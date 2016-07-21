#if !defined(VERTEX_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

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