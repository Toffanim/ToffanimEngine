#if !defined(MATH_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define MATH_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace TE
{
	namespace Math
	{
		//TYpedef to wrap around GLM
		//Integer vectors
		using vec2i = glm::ivec2;
		using vec3i = glm::ivec3;
		using vec4i = glm::ivec4;
		//Floating point vectors
		using vec2f = glm::fvec2;
		using vec3f = glm::fvec3;
		using vec4f = glm::fvec4;
		//Floating point matrices
		using mat2f = glm::fmat2;
		using mat3f = glm::fmat3;
		using mat4f = glm::fmat4;
		

		struct bounding_box
		{
			vec3f UpperBound;
			vec3f LowerBound;
		};

		struct rotation
		{
			float Pitch;
			float Roll;
			float Yaw;
		};

		struct axis3
		{
			vec3f Up;
			vec3f Right;
			vec3f Front;
		};
	}
}

#endif