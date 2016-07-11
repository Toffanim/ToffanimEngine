#if !defined(ACTOR_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator:  $
   $Notice: $
   ======================================================================== */

#define ACTOR_H
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <memory>
#include "../mesh/AABB.h"
#include "../mesh/Model.h"

class Actor
{
public :
	Actor();
    Actor( glm::vec3 position = glm::vec3() );

	glm::mat4& GetTransform();
	void SetTranform(glm::mat4 Transform);

	void Rotate(glm::vec3 Amount, glm::vec3 Axis);
	void Translate(glm::vec3 Amount);
	void Scale(glm::vec3 Amout, glm::vec2 Axis);

	const AABB& GetAABB() const { return(*aabb); };

	void SetModel(Model& M){ model = &M;  ComputeAABB(); }

	void Draw(Shader* s, glm::mat4 WorldToView, glm::mat4 Projection)
	{ 
		auto MV = WorldToView * LocalSpaceToWorldSpace;
		auto MVP = Projection * MV;
		s->SetMatrix4("MVP", MVP);
		s->SetMatrix4("MV", MV);
		model->Draw(s); 
	};

private :
    glm::mat4 LocalSpaceToWorldSpace;
    Model* model;
	//AABB aabb;
    std::unique_ptr<AABB> aabb;
	//std::vector< std::shared_ptr<Actor> > Components;

	void ComputeAABB();

};
#endif
