// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
   $File: $
   $Date: $
   $Modified: 06-02-2017 07h50m20s $
   $Creator:  $
   $Notice: $
   ======================================================================== */
#include "Main.h"
using namespace std;

#include "../engine.h"

//TODO(Marc)

//Get GLFW 3.2
//Get GLEW MX to be able to have multiple window with multiple context (thread safe) ??



//Launch game
int main(int argc, char** argv)
{
	TE::TE_Init();

	TE::Core::window Window(720, 480, "New Window");
	Window.MakeCurrent();

	TE::Renderer::camera_actor Camera;
	mat4f WorldToView = Camera.GetView();

	//Geometry
	int   quad_triangleCount = 2;
	int   quad_triangleList[] = { 0, 1, 2, 2, 1, 3 };
	float quad_vertices[] = { -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0 };
	// Quad
	GLuint vao;
	glGenVertexArrays(1, &vao);
	GLuint vbo[2];
	glGenBuffers(2, vbo);
	glBindVertexArray(vao);
	// Bind indices and upload data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_triangleList), quad_triangleList, GL_STATIC_DRAW);
	// Bind vertices and upload data
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, (void*)0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	TE::Core::texture2D Test("assets/textures/final.png");

	//Shaders
	TE::Renderer::shader BlitShader("RandomName");
	BlitShader.Attach(GL_VERTEX_SHADER, "assets/shaders/blit.vert");
	BlitShader.Attach(GL_FRAGMENT_SHADER, "assets/shaders/blit.frag");
	BlitShader.Link();

	glEnable(GL_DEPTH_TEST);

	Utils::checkGlError("before game loop");
	while (glfwGetKey(Window.GetHandle(), GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		
		//Clean FBOs
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.f, 1.f, 0.0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glViewport(0, 0, Window.GetWidth()/2, Window.GetHeight()/2);

		BlitShader.Use();
		BlitShader.SetInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(vao);
		Test.Bind();
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		
		Window.SwapBuffers();
		Utils::checkGlError("end frame");
	}
    return(0);
}
