// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
   $File: $
   $Date: $
   $Modified: 06-02-2017 07h50m20s $
   $Creator:  $
   $Notice:  $
   ======================================================================== */

#include "Mesh.h"
using namespace std;
//Code from learnopengl.com

/*  Functions  */
// Constructor
Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // Now that we have all the required data, set the vertex buffers and its attribute pointers.
    this->setupMesh();
	Material = {};
}

// Render the mesh
void Mesh::Draw(Shader* shader) 
{
	shader->SetFloat("Material.Metalness", Material.Metalness);
	Utils::checkGlError("rendering gbuffer");
	shader->SetFloat("Material.Roughness", Material.Roughness);
	Utils::checkGlError("rendering gbuffer");
	shader->SetInt("Material.Texture", 0);
	Utils::checkGlError("rendering gbuffer");
	shader->SetVector3("Material.BaseColor", Material.BaseColor);
	Utils::checkGlError("rendering gbuffer");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Material.Texture);
#if 0
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->textures[i].type;
		if (name == "texture_diffuse")
		{
			ss << diffuseNr++; // Transfer GLuint to stream
			glUniform1i(glGetUniformLocation(shader->getProgram(), "Diffuse"), i);
		}
		else if (name == "texture_specular")
		{
			ss << specularNr++; // Transfer GLuint to stream
			glUniform1i(glGetUniformLocation(shader->getProgram(), "Specular"), i);
		}
        else if(name == "texture_normal")
            ss << normalNr++;
        number = ss.str(); 
        // Now set the sampler to the correct texture unit
        //glUniform1i(glGetUniformLocation(shader->getProgram(), std::string("material." + (name + number)).c_str()), i);
        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
#endif        
    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    //glUniform1f(glGetUniformLocation(shader->getProgram(), "material.shininess"), 16.0f);

    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


/*  Functions    */
// Initializes all the buffer objects/arrays
void Mesh::setupMesh()
{
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);   
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glBindVertexArray(0);
	Utils::checkGlError("terrainNormal0");
#if 0
    // Vertex Tangent Space
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
    glBindVertexArray(0);
//Vertex Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent));
    glBindVertexArray(0);
#endif
	
    
}



