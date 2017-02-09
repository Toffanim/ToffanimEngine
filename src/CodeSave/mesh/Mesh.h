// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
/* ========================================================================
   $File: $
   $Modified: 06-02-2017 07h50m20s $
   $Revision: $
   $Creator:  $
   $Notice: $
   ======================================================================== */

#define MESH_H

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
// GL Includes
#include <glew/glew.h> // Contains all the necessery OpenGL includes
#include <assimp/types.h>
#include "../shader/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
    // Tangent
    glm::vec3 Tangent;
    //Bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};

struct material
{
	glm::vec3 BaseColor;
	float Roughness;
	float Metalness;
	float Transparency;
	float Opacity;
	GLuint Texture;
};

class Mesh {
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    /*  Functions  */
    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

    // Render the mesh
    void Draw(Shader *shader);
	void SetMaterial(material& Mat){ Material = Mat; 
	}

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;
	material Material;
    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh();
        
};

#endif
