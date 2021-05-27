// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TECore\math.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 22-08-2017 01h07m00s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(MATH_H)
#define MATH_H

#if 0

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <math.h>

namespace TE
{
	namespace Math
	{
		const double PI = 3.1415926535897932384626433832795;
		const double PIOVER2 = 1.5707963267948966192313216916398;
		const double ONEOVERPI = 0.31830988618379067153776752674503;


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
                return(0);
			}
		};

		struct rotation
		{
			float Pitch = 0.f;
			float Roll = 0.f;
			float Yaw = 0.f;

			mat3f AsRotationMatrix()
			{
				const float lPitch = glm::radians(Pitch);
				const float lYaw = glm::radians(Yaw);
				const float lRoll = glm::radians(Roll);
				const float CosPitch = cos(lPitch);
				const float CosYaw = cos(lYaw);
				const float CosRoll = cos(lRoll);
				const float SinPitch = sin(lPitch);
				const float SinYaw = sin(lYaw);
				const float SinRoll = sin(lRoll);

				return mat3f({ CosPitch*CosYaw, -CosRoll*SinYaw + SinRoll*SinPitch*CosYaw, SinRoll*SinYaw + CosRoll*SinPitch*CosYaw },
				             { CosPitch*SinYaw, CosRoll*CosYaw + SinRoll*SinPitch*SinYaw, -SinRoll*CosYaw + CosRoll*SinPitch*SinYaw },
				             { -SinPitch      , SinRoll*CosPitch                        , CosRoll*CosPitch                          });
			}
		};

		struct axis3
		{
			vec3f Up = vec3f(0.f, 0.f, 1.f);
			vec3f Front = vec3f(0.f, 1.f, 0.f);
			vec3f Right = vec3f(1.f, 0.f, 0.f);
		};

		struct quaternion
		{
			float q0;
			float q1;
			float q2;
			float q3;

			vec4f AsVector()
			{
				return vec4f(q0, q1, q2, q3);
			}

			mat3f AsRotationMatrix()
			{
				const float q0Sq = q0 * q0;
				const float q1Sq = q1 * q1;
				const float q2Sq = q2 * q2;
				const float q3Sq = q3 * q3;
				return mat3f({ q0Sq + q1Sq - q2Sq - q3Sq, 2*( q1*q2 - q0*q3 )      , 2*(q0*q2 + q1*q3)         },
				             { 2*(q1*q2 + q0*q3)        , q0Sq - q1Sq + q2Sq - q3Sq, 2*(q2*q3 - q0*q1)         },
							 { 2*(q1*q3 - q0*q2)        , 2*(q0*q1 + q2*q3)        , q0Sq - q1Sq - q2Sq + q3Sq });
			}
		};

		static rotation QuaternionAsRotation(quaternion Quat)
		{
			const float q1Sq = Quat.q1 * Quat.q1;
			const float q2Sq = Quat.q2 * Quat.q2;
			const float q3Sq = Quat.q3 * Quat.q3;
			rotation Rotation;
			Rotation.Roll = glm::degrees(atan2(2 * (Quat.q0*Quat.q1 + Quat.q2*Quat.q3) , (1 - 2 * (q1Sq + q2Sq))));
			Rotation.Pitch = glm::degrees(asin(2 * (Quat.q0*Quat.q2 + Quat.q3*Quat.q1)));
			Rotation.Yaw = glm::degrees(atan2(2 * (Quat.q0*Quat.q3 + Quat.q1*Quat.q2), (1 - 2 * (q2Sq + q3Sq))));
			return Rotation;
		}

		static quaternion RotationAsQuaternion(rotation Rotation)
		{
			const float YawOver2 = glm::radians(Rotation.Yaw) / 2;
			const float PitchOver2 = glm::radians(Rotation.Pitch) / 2;
			const float RollOver2 = glm::radians(Rotation.Roll) / 2;
			const float CosYawOver2 = cos(YawOver2);
			const float CosPitchOver2 = cos(PitchOver2);
			const float CosRollOver2 = cos(RollOver2);
			const float SinYawOver2 = sin(YawOver2);
			const float SinPitchOver2 = sin(PitchOver2);
			const float SinRollOver2 = sin(RollOver2);
			quaternion Quat;
			Quat.q0 = CosRollOver2*CosPitchOver2*CosYawOver2 + SinRollOver2*SinPitchOver2*SinYawOver2;
			Quat.q1 = SinRollOver2*CosPitchOver2*CosYawOver2 - CosRollOver2*SinPitchOver2*SinYawOver2;
			Quat.q2 = CosRollOver2*SinPitchOver2*CosYawOver2 + SinRollOver2*CosPitchOver2*SinYawOver2;
			Quat.q3 = CosRollOver2*CosPitchOver2*SinYawOver2 - SinRollOver2*SinPitchOver2*CosYawOver2;
			vec4f Vector = glm::normalize(Quat.AsVector());
			Quat.q0 = Vector.x;
			Quat.q1 = Vector.y;
			Quat.q2 = Vector.z;
			Quat.q3 = Vector.w;
			return (Quat);
		}
	}
}
#endif

namespace TE {
	namespace Core {
		namespace Math {
			struct vec2f {
                union {
                    float x;
					float r;
					float w;
					float width;
				};

				union {
					float y;
					float g;
					float h;
					float height;
				}
			};
		}
	}
}


#endif


