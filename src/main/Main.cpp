/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
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
	//Init
	TE::TE_Init();

	//Window
	const int ScreenWidth = 720;
	const int ScreenHeight = 480;
	TE::Core::window Window(ScreenWidth, ScreenHeight, "New Window");
	Window.MakeCurrent();

	//Camera
	TE::Renderer::camera_actor Camera(vec3f(0.f,0.f,10.f));
	TE::Renderer::camera_properties CameraProperties = {};
	CameraProperties.AspectRatio = (float) Window.GetWidth() / Window.GetHeight();
	Camera.SetCameraProperty(CameraProperties);

	//Matrices
	mat4f WorldToView = Camera.GetView();
	mat4f Projection = Camera.GetProjection();
	mat4f MV = Projection * WorldToView;
	mat4f MVP = MV;

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

	//Texture
	TE::Core::texture2D Test("../assets/textures/final.png", "final",
		TE::Core::texture2D::base_internal_format::RGB,
		TE::Core::texture2D::sized_internal_format::SRGB8,
		TE::Core::texture2D::data_type::UNSIGNED_BYTE);

	//Shaders
	TE::Renderer::shader BlitShader("Blit");
	BlitShader.Attach(TE::Renderer::shader::type::VERTEX, "../assets/shaders/blit.vert");
	BlitShader.Attach(TE::Renderer::shader::type::FRAGMENT, "../assets/shaders/blit.frag");
	BlitShader.Link();
	TE::Renderer::shader GBufferShader("GBuffer");
	GBufferShader.Attach(TE::Renderer::shader::type::VERTEX, "../assets/shaders/gbuffer.vert");
	GBufferShader.Attach(TE::Renderer::shader::type::FRAGMENT, "../assets/shaders/gbuffer.frag");
	GBufferShader.Link();

	// FBOs
	TE::Core::fbo GBufferFBO(Window.GetWidth(), Window.GetHeight());
	GBufferFBO.AddDrawBuffer("Color", TE::Core::fbo::attachment::COLOR0,
		TE::Core::texture2D::base_internal_format::RGBA,
		TE::Core::texture2D::sized_internal_format::RGBA32F,
		TE::Core::texture2D::data_type::FLOAT);
	GBufferFBO.AddDrawBuffer("Normals", TE::Core::fbo::attachment::COLOR1,
		TE::Core::texture2D::base_internal_format::RGBA,
		TE::Core::texture2D::sized_internal_format::RGBA32F,
		TE::Core::texture2D::data_type::FLOAT);
	GBufferFBO.AddDepthBuffer(TE::Core::texture2D::sized_internal_format::DEPTH_COMPONENT24);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 1.f, 0.0f, 1.f);

	//Main loop
	while (glfwGetKey(Window.GetHandle(), GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		
		glViewport(0, 0, Window.GetWidth(), Window.GetHeight());
		GBufferFBO.Bind();
		glClearColor(0.f, 1.f, 0.0f, 1.f);
		GBufferFBO.Clear();
		
		GBufferShader.Use();
		GBufferShader.SetMatrix4("MVP", MVP);
		GBufferShader.SetMatrix4("MV", MV);
		GBufferShader.SetInt("ReverseNormals", 0);
		GBufferShader.SetInt("Material.Texture", 0);
		GBufferShader.SetInt("Material.HasTexture", 1);
		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		Test.Bind();
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		

		//Clean FBOs
		TE::Core::fbo::BindDefaultFBO();
		glClearColor(0.f, 1.f, 1.0f, 1.f);
		TE::Core::fbo::ClearDefaultFBO();

		glViewport(0, 0, Window.GetWidth() / 2, Window.GetHeight() / 2);
		BlitShader.Use();
		BlitShader.SetInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(vao);
		GBufferFBO.BindTexture("Color");
		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (void*)0);
		
		Window.SwapBuffers();
		Utils::checkGlError("end frame");
	}
    return(0);
}
