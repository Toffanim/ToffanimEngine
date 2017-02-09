// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
/* ========================================================================
$File: $
$Modified: 08-02-2017 12h59m38s $
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
// TODO(Marc) :
class Actor
{
    public :
        Actor();
        Actor( glm::vec3 position = glm::vec3() );

        glm::mat4& GetTransform();
        void SetTranform(glm::mat4 Transform);

        void Rotate(glm::vec3 mount, glm::vec3 Axis);
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
