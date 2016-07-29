#include "TEPhysics\physic.h"

namespace TE
{
	namespace Physic
	{
		/*
		*
		*
		*
		*/
		// Temporay class declaration
		CVector::CVector()
		{
			xv = 0;
			yv = 0;
		}


		CVector::~CVector()
		{
		}
		//end of temporary declarations
		/*
		*
		*
		*
		*/
		bool DoubleJumpControler()
		{
			// temporary shits for code adaptation to be remove after actor class rebuild 


			// Game Actor datas
			bool JumpFlag, SpinFlag, SlamFlag, DoubleJumpFlag, DoubleJumpRequestFlag;
			int JumpEvolutionTracer;
			// end of temp 


			// #1  need to be move to core actor func
			if ((DoubleJumpFlag == false) && (DoubleJumpRequestFlag == true))// Reinit for actor double jump
			{
				//validating double jump
				DoubleJumpFlag = true;
				DoubleJumpRequestFlag = false;
				//
				JumpEvolutionTracer = 0;
			}
			// #1

			return true;
		}
		int AirEvolution(TE::Core::actor & CoreActorA, CVector ActorLocalGravity)
		{
			/* Require a actor object (both core and game)  and the map gravity matrix (or equivalent) 
			Function on updating current actor speed vector, and airevolutiontracer
			Will return displacement of the actor */
			
			/*
			*
			*
			*
			*/
			// temporary shits for code adaptation to be remove after actor class rebuild 
					
			
			// Game Actor datas
			bool JumpFlag, SpinFlag, SlamFlag, DoubleJumpFlag, DoubleJumpRequestFlag;
			float ActorInitialVerticalSpeed, ActorInitialSpinSpeed;

			// Core Actor datas
			CVector ActorCurrentXYSpeed;
			CVector Center;
			float ActorCurrentSpinSpeed;
			int JumpEvolutionTracer, SpinEvolutionTracer;
				// physics constants
			double sTimeConstant = 0.016;
			
			// dont know data storage
			float ActorMass, ActorCollisionRestitutionCoefficient;

			
			// end of temporary declarations
			/*
			*
			*
			*
			*/
			
			
			
			float JumpTime = 0;
				CVector DeltaActorCoordinate;
				// Jump routine
				if (/*GameActorA.*/JumpFlag == true)
				{


					// Calculate jump statut in time scale
					JumpTime = float(JumpEvolutionTracer)*float(sTimeConstant);// Calculate jump statut in time scale

					// Evolution of  Actor vectical speed
					ActorCurrentXYSpeed.yv = ActorInitialVerticalSpeed*exp((-1 * JumpTime) / ActorMass);


					// Calculating Actor deltaX coordinate
					DeltaActorCoordinate.xv = sTimeConstant*ActorLocalGravity.xv*ActorMass;// + ActorControllerXMouvement 
					
					// Calculating Actor deltaY coordinate
					DeltaActorCoordinate.yv = float(10.0 * (sTimeConstant*((ActorCurrentXYSpeed.yv*ActorLocalGravity.yv*ActorMass) - (ActorLocalGravity.yv*ActorMass)))); // Calculating Player deltaY coordinate 


																																										  // Incrementing jump statut
					JumpEvolutionTracer++;
				}


				return 1;
				//return DeltaActorCoordinate;

			}
		
	}
}