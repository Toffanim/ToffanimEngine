/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator:  $
   $Notice: $
   ======================================================================== */
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "engine.h"
using namespace TE;// <= Using namesapce TE to save time

//TODO(Marc)

//Get GLFW 3.2
//Get GLEW MX to be able to have multiple window with multiple context (thread safe) ??


static bool Continue = true;

void CloseApp()
{
	Continue = false;
}

//Launch game
int main(int argc, char** argv)
{
	// TE_Init() : Init every global variables, creating a window, some fbos and shaders
	//
	// Window creation example
	// -------------------------------
	// static unsigned int ScreenWidth = 720;
	// static unsigned int ScreenHeight = 720;
	// static Core::window Window(ScreenWidth, ScreenHeight, "New Window");
	// Window.MakeCurrent();
	//
	// FBO creation example
	// -------------------------------
	// static Core::fbo GBufferFBO(ScreenWidth, ScreenHeight);
	// GBufferFBO.AddDrawBuffer("Color", Core::fbo::attachment::COLOR0,
	//	Core::texture2D::base_internal_format::RGBA,
	//	Core::texture2D::sized_internal_format::RGBA32F,
	//	Core::texture2D::data_type::FLOAT);
	// GBufferFBO.AddDrawBuffer("Normals", Core::fbo::attachment::COLOR1,
	//	Core::texture2D::base_internal_format::RGBA,
	//	Core::texture2D::sized_internal_format::RGBA32F,
	//	Core::texture2D::data_type::FLOAT);
	// GBufferFBO.AddDepthBuffer(Core::texture2D::sized_internal_format::DEPTH_COMPONENT24);
	//
	// Input handler creation example
    // -------------------------------
	// static Core::input_handler DefaultInputHandler;
	//
	// Shader creation example
	// -------------------------------
	// static Renderer::shader GBufferShader("Gbuffer");
	//	GBufferShader.Attach(Renderer::shader::type::VERTEX, "../assets/shaders/gbuffer.vert");
	//  GBufferShader.Attach(Renderer::shader::type::FRAGMENT, "../assets/shaders/gbuffer.frag");
	//  GBufferShader.Link();

	TE_Init();

	//Camera
	Renderer::camera_actor Camera(vec3f(0.f,0.f,10.f));
	Renderer::camera_properties CameraProperties = {};
	CameraProperties.AspectRatio = (float)TE::Window->GetWidth() / TE::Window->GetHeight();
	Camera.SetCameraProperty(CameraProperties);
	// Input component example
	std::shared_ptr<Core::input_component> InputComponent = std::make_shared<Core::input_component>();
	InputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_ESCAPE, GLFW_PRESS), &CloseApp);
	TE::DefaultInputHandler->Add(InputComponent);
	InputComponent->SetupAttachement(Camera.Root());
	InputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_W, GLFW_PRESS), std::bind(&Renderer::camera_actor::AddVector, &Camera, vec3f(0.f,0.f,1.f)));
	InputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_A, GLFW_PRESS), std::bind(&Renderer::camera_actor::AddVector, &Camera, vec3f(1.f, 0.f, 0.f)));
	InputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_S, GLFW_PRESS), std::bind(&Renderer::camera_actor::AddVector, &Camera, vec3f(0.f, 0.f, -1.f)));
	InputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_D, GLFW_PRESS), std::bind(&Renderer::camera_actor::AddVector, &Camera, vec3f(-1.f, 0.f, 0.f)));

	//Texture
	Core::texture2D Test("../assets/textures/final.png", "final",
		Core::texture2D::base_internal_format::RGB,
		Core::texture2D::sized_internal_format::SRGB8,
		Core::texture2D::data_type::UNSIGNED_BYTE);
	//Sprite
	Core::actor Sprite;
	Renderer::sprite SpriteComponent(Test);
	SpriteComponent.SetupAttachement(&Sprite);
	SpriteComponent.SetSize({ 1.f, 1.f });
	//Geometry
	std::vector<Core::vertex> Vertices;
	Vertices.push_back({ { -1.f, -1.f, 0.f },  //Position
	                     { 0.f, 0.f, 1.f },    // Normal
	                     { 0.f, 0.f } });      // TexCoorsds
	Vertices.push_back({ { 1.f, -1.f, 0.f },  
	                     { 0.f, 0.f, 1.f },    
	                     { 1.f, 0.f } }); 
	Vertices.push_back({ { -1.f, 1.f, 0.f },
	                     { 0.f, 1.f, 1.f },
	                     { 0.f, 1.f } });
	Vertices.push_back({ { 1.f, 1.f, 0.f },
	                     { 0.f, 0.f, 1.f },
	                     { 1.f, 1.f } });

	std::vector<Core::vertex> Vertices2;
	Vertices2.push_back({ { -1.f, -1.f, 0.f },  //Position
	{ 0.f, 0.f, 1.f },    // Normal
	{ 0.f, 0.f } });      // TexCoorsds
	Vertices2.push_back({ { 1.f, -1.f, 0.f },
	{ 0.f, 0.f, 1.f },
	{ 1.f, 0.f } });
	Vertices2.push_back({ { -1.f, 1.f, 0.f },
	{ 0.f, 1.f, 1.f },
	{ 0.f, 1.f } });
	Vertices2.push_back({ { 1.f, 1.f, 0.f },
	{ 0.f, 0.f, 1.f },
	{ 1.f, 1.f } });
	std::vector<unsigned int> Indices = { 0, 1, 2, 2, 1, 3 };
	Core::vertex_array Quad(Vertices, Indices);
	Core::vertex_array UnitQuad(Vertices2, Indices);



	//Shaders
	Renderer::shader BlitShader("Blit");
	BlitShader.Attach(Renderer::shader::type::VERTEX, "../assets/shaders/blit.vert");
	BlitShader.Attach(Renderer::shader::type::FRAGMENT, "../assets/shaders/blit.frag");
	BlitShader.Link();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 1.f, 0.0f, 1.f);

	//Main loop
	while (Continue)
	{
		if (glfwGetTime() > 5)
		{
			for (int i = 0; i < Vertices.size(); ++i)
			{
				Vertices[i].Position = Vertices2[i].Position * vec3f(std::sin(glfwGetTime())+2);
			}
			Quad.Update(Vertices);
		}
		//Matrices
		mat4f View = Camera.GetView();
		mat4f Projection = Camera.GetProjection();
		mat4f MV = Projection * View;
		mat4f MVP = MV;

		glViewport(0, 0, TE::Window->GetWidth(), TE::Window->GetHeight());
		TE::GBufferFBO->Bind();
		glClearColor(0.f, 1.f, 0.0f, 1.f);
		TE::GBufferFBO->Clear();

		Sprite.Render( Projection, View );


		//Clean FBOs
		Core::frame_buffer::BindDefaultFBO();
		glClearColor(0.f, 1.f, 1.0f, 1.f);
		Core::frame_buffer::ClearDefaultFBO();

		glViewport(0, 0, TE::Window->GetWidth() / 2, TE::Window->GetHeight() / 2);
		BlitShader.Bind();
		BlitShader.SetInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		TE::GBufferFBO->BindTexture("Color");
		UnitQuad.Render();
		

		TE::Window->SwapBuffers();
		Core::CheckOpenGLError("end frame");
	}
    return(0);
}
