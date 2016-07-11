#if !defined(AABB_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator:  $
   $Notice: $
   ======================================================================== */

#define AABB_H
#include <glm/glm.hpp>

//Simply the AABB structure
struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 size;
    glm::vec3 center;

	int 
	MainAxis() const
	{
		auto DiffX = max.x - min.x;
		auto DiffY = max.y - min.y;
		auto DiffZ = max.z - min.z;
		if (DiffX > DiffY || DiffX > DiffZ)
			return 0;
		if (DiffY > DiffX || DiffY > DiffZ)
			return 1;
		if (DiffZ > DiffX || DiffZ > DiffY)
			return 2;
	}
};
#endif
