/* ========================================================================
    $File: TECore\globals.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 02-02-2017 11h57m31s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#if !defined(GLOBALS_H)
#define GLOBALS_H

//Forward declarations
namespace TE
{
	namespace Renderer
	{
		class shader;
	}

	namespace Core
	{
		class window;
		class input_handler;
		class frame_buffer;
		class actor;
	}
}
#include <vector>

//Global variables
// NOTE(Marc) : DEFINE VARIABLES IN "initialization.h"
//
// IMPORTANT(Marc) : Move this global variables when the time is needed
// meaning prolly create an object "game" or "engine"
namespace TE
{
	extern unsigned int ScreenWidth;
	extern unsigned int ScreenHeight;
	extern Renderer::shader* GBufferShader;
	extern Core::frame_buffer* GBufferFBO;
	extern Core::input_handler* DefaultInputHandler;
	extern Core::window* Window;
	extern std::vector<Core::actor> Scene;
}

#endif