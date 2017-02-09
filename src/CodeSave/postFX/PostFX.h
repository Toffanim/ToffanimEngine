// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
/* ========================================================================
   $File: $
   $Modified: 06-02-2017 07h50m20s $
   $Revision: $
   $Creator: Marc TOFFANIN $
   $Notice: Given as-is without warranty AT ALL that it's working$
   ======================================================================== */

#define POSTFX_H

#include "glew/glew.h"
#include "../shader/Shader.h"
#include "../utils/Utils.h"
namespace PostFX
{
    GLuint blur(Shader s, GLuint texture, int amount);    
}
#endif
