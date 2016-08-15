#if !defined(INITIALISATION_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */
#define INITIALISATION_H

//NOTE(Marc) : Always call glew before any openGL context creation (GLFW, SDL, etc...)
#include "GL/glew.h"

#define USE_GLFW

#if defined(USE_GLFW)
#include "GLFW/glfw3.h"
#endif

#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "input_handler.h"
#include "window.h"
#include "frame_buffer.h"
#include "TERenderer\shader.h"
#include "globals.h"

namespace TE
{
	unsigned int TE::ScreenWidth ;
	unsigned int TE::ScreenHeight;
	Core::input_handler* TE::DefaultInputHandler;
	Core::window* TE::Window;
	Core::frame_buffer * TE::GBufferFBO;
	Renderer::shader* TE::GBufferShader;
	//TODO(Marc) : Make this return a value for error or success
	//Initialize third party like GLFW, SDL, GLEW, etc
	//THe call to this function is necessary to use the engine
	void TE_Init()
	{
        #if defined(USE_GLFW)
		if (!glfwInit())
		{
			std::cerr << "ERROR : Failed to initialize GLFW" << std::endl;
			return;
		}
        #endif
		
		//Initialize global variables
		TE::DefaultInputHandler = new Core::input_handler();
		TE::ScreenWidth = 720;
		TE::ScreenHeight = 480;
		TE::Window = new Core::window(TE::ScreenWidth, TE::ScreenHeight);
		TE::Window->MakeCurrent();
		// print diagnostic information
		printf("    GL VENDOR: %s\n", glGetString(GL_VENDOR));
		printf("      VERSION: %s\n", glGetString(GL_VERSION));
		printf("     RENDERER: %s\n", glGetString(GL_RENDERER));
		printf("GLSL VERSION : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		TE::GBufferFBO = new Core::frame_buffer(TE::ScreenWidth, TE::ScreenHeight);
		TE::GBufferFBO->AddDrawBuffer("Colors", Core::frame_buffer::attachment::COLOR0,
			Core::texture2D::base_internal_format::RGBA,
			Core::texture2D::sized_internal_format::RGBA32F,
			Core::texture2D::data_type::FLOAT);
		TE::GBufferFBO->AddDrawBuffer("Normals", Core::frame_buffer::attachment::COLOR1,
			Core::texture2D::base_internal_format::RGBA,
			Core::texture2D::sized_internal_format::RGBA32F,
			Core::texture2D::data_type::FLOAT);
		TE::GBufferFBO->AddDepthBuffer(Core::texture2D::sized_internal_format::DEPTH_COMPONENT24);
		TE::GBufferShader = new Renderer::shader("Gbuffer");
		TE::GBufferShader->Attach(Renderer::shader::type::VERTEX, "../assets/shaders/gbuffer.vert");
		TE::GBufferShader->Attach(Renderer::shader::type::FRAGMENT, "../assets/shaders/gbuffer.frag");
		TE::GBufferShader->Link();

		Core::CheckOpenGLError("GLEW INIT (Safe To Ignore)");

		// test if we've got GL 3.0 and forward (We need at least > 2.0)
		if (!GLEW_VERSION_3_0)
		{
			printf("OpenGL 3.0 not supported.\n"
				"Please update your drivers and/or buy a better graphics card."
				);
		}
		Core::CheckOpenGLError("Destroy Window");
	}
}

#endif