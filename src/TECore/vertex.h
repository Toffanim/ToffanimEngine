// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\vertex.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 09h56m57s $
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
