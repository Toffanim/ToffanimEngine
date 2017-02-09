// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TERenderer\skybox.cpp $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 06-02-2017 07h50m25s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#include "Skybox.h"
using namespace std;

namespace TE
{
	namespace Renderer
	{
		skybox::skybox(const vector<const GLchar*>& faces)
		{
			std::vector<Core::vertex> Vertices({
				// Positions          
					{ { -1.0f, 1.0f, -1.0f } },
					{ { -1.0f, -1.0f, -1.0f } },
					{ { 1.0f, -1.0f, -1.0f } },
					{ { 1.0f, -1.0f, -1.0f } },
					{ { 1.0f, 1.0f, -1.0f } },
					{ { -1.0f, 1.0f, -1.0f } },

					{ { -1.0f, -1.0f, 1.0f } },
					{ { -1.0f, -1.0f, -1.0f } },
					{ { -1.0f, 1.0f, -1.0f } },
					{ { -1.0f, 1.0f, -1.0f } },
					{ { -1.0f, 1.0f, 1.0f } },
					{ { -1.0f, -1.0f, 1.0f } },

					{ { 1.0f, -1.0f, -1.0f } },
					{ { 1.0f, -1.0f, 1.0f } },
					{ { 1.0f, 1.0f, 1.0f } },
					{ { 1.0f, 1.0f, 1.0f } },
					{ { 1.0f, 1.0f, -1.0f } },
					{ { 1.0f, -1.0f, -1.0f } },

					{ { -1.0f, -1.0f, 1.0f } },
					{ { -1.0f, 1.0f, 1.0f } },
					{ { 1.0f, 1.0f, 1.0f } },
					{ { 1.0f, 1.0f, 1.0f } },
					{ { 1.0f, -1.0f, 1.0f } },
					{ { -1.0f, -1.0f, 1.0f } },

					{ { -1.0f, 1.0f, -1.0f } },
					{ { 1.0f, 1.0f, -1.0f } },
					{ { 1.0f, 1.0f, 1.0f } },
					{ { 1.0f, 1.0f, 1.0f } },
					{ { -1.0f, 1.0f, 1.0f } },
					{ { -1.0f, 1.0f, -1.0f } },

					{ { -1.0f, -1.0f, -1.0f } },
					{ { -1.0f, -1.0f, 1.0f } },
					{ { 1.0f, -1.0f, -1.0f } },
					{ { 1.0f, -1.0f, -1.0f } },
					{ { -1.0f, -1.0f, 1.0f } },
					{ { 1.0f, -1.0f, 1.0f } }
			});
            _VAO = new Core::vertex_array(Vertices);

			_Shader = new TE::Renderer::shader("Skybox shader");
			_Shader->Attach(TE::Renderer::shader::type::VERTEX, "../assets/shaders/skybox.vert");
			_Shader->Attach(TE::Renderer::shader::type::FRAGMENT, "../assets/shaders/skybox.frag");
			_Shader->Link();
			_TextureID = loadCubemap(faces);
		}

		skybox::~skybox()
		{
			glDeleteTextures(1, &_TextureID);
			delete _Shader;
			delete _VAO;
		}


		// Loads a cubemap texture from 6 individual texture faces
		// Order should be:
		// +X (right)
		// -X (left)
		// +Y (top)
		// -Y (bottom)
		// +Z (front)
		// -Z (back)
		GLuint skybox::loadCubemap(const vector<const GLchar*>& faces)
		{
			GLuint textureID;
			glGenTextures(1, &textureID);

			int x, y, comp;
			std::unique_ptr<unsigned char> image;

			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			for (GLuint i = 0; i < faces.size(); i++)
			{
				image = TE::Core::LoadImage(faces[i], &x, &y, &comp, STBI_rgb);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB8, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image.get());
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			return textureID;
		}


		void skybox::display(glm::mat4 view, glm::mat4 proj)
		{
			//glDisable(GL_DEPTH_TEST);
			_Shader->Bind();
			_Shader->SetMatrix4("view", view);
			_Shader->SetMatrix4("projection", proj);
			//glUniform3f(glGetUniformLocation(shader->getProgram(), "sun"), sunDir.x, sunDir.y, sunDir.z);
			// skybox cube
			glActiveTexture(GL_TEXTURE0);
			_Shader->SetInt("skybox", 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _TextureID);
			_VAO->Render();
			//glDepthFunc(GL_LESS); // Set depth function back to default
		}

	}
}
