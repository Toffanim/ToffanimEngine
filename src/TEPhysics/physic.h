#if !defined(PHYSIC_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define PHYSIC_H
#include "TECore/actor.h"


namespace TE
{
	namespace Physic
	{
		// Temporary class declaration 
		class CVector
		{
		public:
			float xv, yv;
		public:
			CVector();
			virtual ~CVector();
		};

		int AirEvolution(TE::Core::actor & , CVector);
	}
}
#endif