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
#include <math.h>

namespace TE
{
	namespace Math
	{
		//TYPEDEF TO WRAP AROUND GLM
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
			vec3f Center;

			int MainAxis() const
			{
				auto DiffX = UpperBound.x - LowerBound.x;
				auto DiffY = UpperBound.y - LowerBound.y;
				auto DiffZ = UpperBound.z - LowerBound.z;
				if (DiffX > DiffY || DiffX > DiffZ)
					return 0;
				if (DiffY > DiffX || DiffY > DiffZ)
					return 1;
				if (DiffZ > DiffX || DiffZ > DiffY)
					return 2;
			}
		};

		struct rotation
		{
			float Pitch;
			float Roll;
			float Yaw;
		};

		struct axis3
		{
			vec3f Up = vec3f(0.f, 1.f, 0.f);
			vec3f Right = vec3f(1.f, 0.f, 0.f);
			vec3f Front = vec3f(0.f, 0.f, -1.f);;
		};
	}
}

#endif