#if !defined(SKYBOX_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Marc TOFFANIN $
   $Notice: Given as-is without warranty AT ALL that it's working$
   ======================================================================== */

#define SKYBOX_H
#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#include "TERenderer\shader.h"
#include "TECore\utils.h"

class Skybox
{
public :
    Skybox( const std::vector< const GLchar*>& faces );
    ~Skybox();
	void display(glm::mat4 view, glm::mat4 proj);
	GLuint texture_id;
private :
    GLuint loadCubemap( const std::vector<const GLchar*>& faces );
    GLuint VAO;
    GLuint VBO;
    TE::Renderer::shader* Shader;
};
#endif
