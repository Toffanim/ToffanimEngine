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

#include "Skybox.h"
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

	//Texture
	Core::texture2D Test("../assets/textures/spnza_bricks_a_diff.tga", "final",
		Core::texture2D::base_internal_format::RGB,
		Core::texture2D::sized_internal_format::SRGB8,
		Core::texture2D::data_type::UNSIGNED_BYTE);
	//Sprite actor
	Core::actor Sprite;
	// Sprite component
	Renderer::sprite SpriteComponent(Test);
	SpriteComponent.SetupAttachement(Sprite.GetRoot());
	SpriteComponent.SetSize({ 1.f, 1.f });
	// Input component
#if 0
	std::shared_ptr<Core::input_component> SpriteInputComponent = std::make_shared<Core::input_component>();
	SpriteInputComponent->Init();
	SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_W, GLFW_PRESS), std::bind(&Core::actor::AddVector, &Sprite, vec3f(0.f, 0.f, 1.f)));
	SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_A, GLFW_PRESS), std::bind(&Core::actor::AddVector, &Sprite, vec3f(-1.f, 0.f, 0.f)));
	SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_S, GLFW_PRESS), std::bind(&Core::actor::AddVector, &Sprite, vec3f(0.f, 0.f, -1.f)));
	SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_D, GLFW_PRESS), std::bind(&Core::actor::AddVector, &Sprite, vec3f(1.f, 0.f, 0.f)));
	SpriteInputComponent->SetupAttachement(Sprite.GetRoot());
#endif
	// Camera component centered on the sprite 
	Renderer::camera_component CameraComponent;
	CameraComponent.SetLocalPosition({ 0, 10, 0 });
	Renderer::camera_properties CameraProperties = {};
	CameraProperties.AspectRatio = (float)TE::Window->GetWidth() / TE::Window->GetHeight();
	CameraComponent.SetCameraProperty(CameraProperties);
	CameraComponent.SetupAttachement(Sprite.GetRoot());

	vector<const GLchar*> faces;
	faces.push_back("../assets/skyboxes/Test/xpos.png");
	faces.push_back("../assets/skyboxes/Test/xneg.png");
	faces.push_back("../assets/skyboxes/Test/ypos.png");
	faces.push_back("../assets/skyboxes/Test/yneg.png");
	faces.push_back("../assets/skyboxes/Test/zpos.png");
	faces.push_back("../assets/skyboxes/Test/zneg.png");
	Skybox* skybox = new Skybox(faces);


	Renderer::camera_actor FreeCam({ 0, 10, 10 });
	FreeCam.SetCameraProperty(CameraProperties);
	std::shared_ptr<Core::input_component> FreeCamInput = std::make_shared<Core::input_component>();
	FreeCamInput->Init();
	FreeCamInput->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_W, GLFW_PRESS), std::bind(&Core::actor::AddVector, &FreeCam, vec3f(0.f, 0.f, 1.f)));
	FreeCamInput->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_A, GLFW_PRESS), std::bind(&Core::actor::AddVector, &FreeCam, vec3f(-1.f, 0.f, 0.f)));
	FreeCamInput->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_S, GLFW_PRESS), std::bind(&Core::actor::AddVector, &FreeCam, vec3f(0.f, 0.f, -1.f)));
	FreeCamInput->AddKeyBind(std::make_pair<int, int>(GLFW_KEY_D, GLFW_PRESS), std::bind(&Core::actor::AddVector, &FreeCam, vec3f(1.f, 0.f, 0.f)));
	FreeCamInput->AddCursorBind(std::bind(&Renderer::camera_actor::HandleCursor, &FreeCam, std::placeholders::_1, std::placeholders::_2));
	FreeCamInput->SetupAttachement(FreeCam.GetRoot());

	//Background Sprite
	Core::actor SpriteBackground;
	// Sprite component
	Renderer::sprite SpriteBackgroundComponent(Test);
	SpriteBackgroundComponent.SetupAttachement(SpriteBackground.GetRoot());
	SpriteBackgroundComponent.SetSize({ 20.f, 20.f });


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
	std::vector<unsigned int> Indices = { 0, 1, 2, 2, 1, 3 };
	Core::vertex_array UnitQuad(Vertices, Indices);



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
		//Matrices
		mat4f View = FreeCam.GetView();
		mat4f Projection = FreeCam.GetProjection();
		mat4f MV = Projection * View;
		mat4f MVP = MV;

		glViewport(0, 0, TE::Window->GetWidth(), TE::Window->GetHeight());
		TE::GBufferFBO->Bind();
		glClearColor(0.f, 1.f, 0.0f, 1.f);
		TE::GBufferFBO->Clear();

		//Sprite.Render( Projection, View );
		SpriteBackground.Render(Projection, View);

		View = glm::mat4(glm::mat3(FreeCam.GetView()));    // Remove any translation component of the view matrix
		//skybox->display(View, Projection, TE::GBufferFBO->GetDepthTextureID(), TE::Window->GetWidth(), TE::Window->GetHeight());


		//Clean FBOs
		Core::frame_buffer::BindDefaultFBO();
		glClearColor(0.f, 1.f, 1.f, 1.f);
		Core::frame_buffer::ClearDefaultFBO();

		glViewport(0, 0, TE::Window->GetWidth() / 2, TE::Window->GetHeight() / 2);
		BlitShader.Bind();
		BlitShader.SetInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		TE::GBufferFBO->BindTexture("Colors");
		//TE::GBufferFBO->BindTexture("Normals");
		//TE::GBufferFBO->BindDepthTexture();
		UnitQuad.Render();
		

		TE::Window->SwapBuffers();
		Core::CheckOpenGLError("end frame");
	}
    return(0);
}
