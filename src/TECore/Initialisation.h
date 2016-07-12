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

namespace TE
{
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
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		//Make temporary window to create a context and bein able to
		//Initialize GLEW and get infos
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		GLFWwindow* tmp = glfwCreateWindow(1, 1, "", NULL, NULL);
		glfwMakeContextCurrent(tmp);
        #endif

		
		//NOTE(Marc) : Don't forget to fully create context before initializing GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}
		
		// print diagnostic information
		printf("    GL VENDOR: %s\n", glGetString(GL_VENDOR));
		printf("      VERSION: %s\n", glGetString(GL_VERSION));
		printf("     RENDERER: %s\n", glGetString(GL_RENDERER));
		printf("GLSL VERSION : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		// test if we've got GL 3.0 and forward (We need at least > 2.0)
		if (!GLEW_VERSION_3_0)
		{
			printf("OpenGL 3.0 not supported.\n"
				"Please update your drivers and/or buy a better graphics card."
				);
		}

		glfwDestroyWindow(tmp);
	}
}

#endif