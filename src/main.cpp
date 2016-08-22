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

// ------ Multiple window with multiple contexts ------
// Get GLFW 3.2
// Get GLEW MX to be able to have multiple window with multiple context (thread safe) ??
// ------ Coordinate system ------
//Check rotations in good coordinate system


static bool Continue = true;
static bool DebugMode = false;

void CloseApp()
{
	Continue = false;
}

void ToggleDebugMode()
{
	DebugMode = true;
}

//Launch game
int main(int argc, char** argv)
{
#if 1
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
	Core::texture2D Test2("../assets/textures/final.png", "final",
		Core::texture2D::base_internal_format::RGB,
		Core::texture2D::sized_internal_format::SRGB8,
		Core::texture2D::data_type::UNSIGNED_BYTE);
	//Sprite actor
	std::shared_ptr<Core::actor> Sprite = std::make_shared<Core::actor>();
	// Sprite component
	Renderer::sprite SpriteComponent(Test2);
	SpriteComponent.SetSize({ 1.f, 1.f });
	SpriteComponent.SetupAttachement(Sprite->GetRoot());
	// Input component
	std::shared_ptr<Core::input_component> SpriteInputComponent = std::make_shared<Core::input_component>();
	//SpriteInputComponent->Init();
	SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(TE::Core::W, TE::Core::PRESS), std::bind(&Core::actor::AddVector, Sprite, vec3f(0.f, 1.f, 0.f)));
	SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(TE::Core::A, TE::Core::PRESS), std::bind(&Core::actor::AddVector, Sprite, vec3f(-1.f, 0.f, 0.f)));
	SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(TE::Core::S, TE::Core::PRESS), std::bind(&Core::actor::AddVector, Sprite, vec3f(0.f, -1.f, 0.f)));
	SpriteInputComponent->AddKeyBind(std::make_pair<int, int>(TE::Core::D, TE::Core::PRESS), std::bind(&Core::actor::AddVector, Sprite, vec3f(1.f, 0.f, 0.f)));
	//SpriteInputComponent->AddCursorBind(std::bind(&Renderer::camera_actor::HandleCursor, &FreeCam, std::placeholders::_1, std::placeholders::_2));
	SpriteInputComponent->SetupAttachement(Sprite->GetRoot());
	// Camera component centered on the sprite 
	Renderer::camera_component CameraComponent;
	CameraComponent.SetLocalPosition({ 0, -1, 0 });
	Renderer::camera_properties CameraProperties = {};
	CameraProperties.AspectRatio = (float)TE::Window->GetWidth() / TE::Window->GetHeight();
	CameraComponent.SetCameraProperty(CameraProperties);
	CameraComponent.SetupAttachement(Sprite->GetRoot());

	vector<const GLchar*> faces;
	faces.push_back("../assets/skyboxes/Test/xpos.png");
	faces.push_back("../assets/skyboxes/Test/xneg.png");
	faces.push_back("../assets/skyboxes/Test/ypos.png");
	faces.push_back("../assets/skyboxes/Test/yneg.png");
	faces.push_back("../assets/skyboxes/Test/zpos.png");
	faces.push_back("../assets/skyboxes/Test/zneg.png");
	Skybox* skybox = new Skybox(faces);


	Renderer::camera_actor FreeCam({ 0, -2, 0 });
	FreeCam.SetCameraProperty(CameraProperties);
	std::shared_ptr<Core::input_component> FreeCamInput = std::make_shared<Core::input_component>();
	FreeCamInput->Init();
	FreeCamInput->AddKeyBind(std::make_pair<int, int>(TE::Core::W, TE::Core::PRESS), std::bind(&Core::actor::AddVector, &FreeCam, vec3f(0.f, 1.f, 0.f)));
	FreeCamInput->AddKeyBind(std::make_pair<int, int>(TE::Core::A, TE::Core::PRESS), std::bind(&Core::actor::AddVector, &FreeCam, vec3f(-1.f, 0.f, 0.f)));
	FreeCamInput->AddKeyBind(std::make_pair<int, int>(TE::Core::S, TE::Core::PRESS), std::bind(&Core::actor::AddVector, &FreeCam, vec3f(0.f, -1.f, 0.f)));
	FreeCamInput->AddKeyBind(std::make_pair<int, int>(TE::Core::D, TE::Core::PRESS), std::bind(&Core::actor::AddVector, &FreeCam, vec3f(1.f, 0.f, 0.f)));
	FreeCamInput->AddCursorBind(std::bind(&Renderer::camera_actor::HandleCursor, &FreeCam, std::placeholders::_1, std::placeholders::_2));
	FreeCamInput->SetupAttachement(FreeCam.GetRoot());

	//Background Sprite
	std::shared_ptr<Core::actor> SpriteBackground = std::make_shared<Core::actor>();
	// Sprite component
	Renderer::sprite SpriteBackgroundComponent(Test);
	SpriteBackgroundComponent.SetSize({ 20.f, 20.f });
	SpriteBackgroundComponent.SetupAttachement(SpriteBackground->GetRoot());



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

	// TODO (MArc) : Make an object "Renderer" so we can enable/disable option directly on it
	// Maybe even register the renderable objects
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.f, 1.f, 0.0f, 1.f);

	//Physic
	Physic::bvh_object_list SceneActors;
	SceneActors.push_back(Sprite);
	SceneActors.push_back(SpriteBackground);
	// BVH construction
	Physic::bvh SceneTree(SceneActors);
	// Ray intersection
	Physic::ray Ray;
	Ray.Origin = Math::vec3f(0.f, -2.f, 0.f);
	Ray.Direction = Math::vec3f( 0.f, 1.f, 0.f );
	Ray.InverseDirection = 1.f / Ray.Direction;
	Ray.T = 1000000;
	SceneTree.GetIntersection(Ray);


	std::shared_ptr<Core::input_component> WindowController = std::make_shared<Core::input_component>();
	WindowController->Init();
	WindowController->AddKeyBind(std::make_pair<int, int>(TE::Core::ESCAPE, TE::Core::PRESS), &CloseApp);
	WindowController->AddKeyBind(std::make_pair<int, int>(TE::Core::KP_1, TE::Core::PRESS), std::bind(&Core::window::SetFullScreen, Window));
	WindowController->AddKeyBind(std::make_pair<int, int>(TE::Core::KP_2, TE::Core::PRESS), std::bind(&Core::window::SetWindowed, Window));
	WindowController->AddKeyBind(std::make_pair<int, int>(TE::Core::D, TE::Core::PRESS), &ToggleDebugMode);

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

		// Render Atmosphere
		mat4f ViewNoTrans = glm::mat4(glm::mat3(FreeCam.GetView()));    // Remove any translation component of the view matrix
		skybox->display(ViewNoTrans, Projection);
		TE::GBufferFBO->Clear(Core::frame_buffer::CLEAR_DEPTH);
		//Render scene geometry
		Sprite->Render( Projection, View );
		SpriteBackground->Render(Projection, View);

		//Clean default FBOs
		Core::frame_buffer::BindDefaultFBO();
		glClearColor(0.f, 1.f, 1.f, 1.f);
		Core::frame_buffer::ClearDefaultFBO();
		// Blit from GBuffer to default FBO
		BlitShader.Bind();
		BlitShader.SetInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		TE::GBufferFBO->BindTexture("Colors");
		//TE::GBufferFBO->BindTexture("Normals");
		//TE::GBufferFBO->BindDepthTexture();
		UnitQuad.Render();

		if (DebugMode)
		{
			ImGui::NewFrame();

			ImGui::Render();
		}
		

		TE::Window->SwapBuffers();
		Core::CheckOpenGLError("end frame");
	}

	delete skybox;
#endif
	TE_Clean();
    return(0);
}
