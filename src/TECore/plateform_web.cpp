#include <EGL/egl.h>
#include <iostream>
#include <emscripten.h>
#include <emscripten/html5.h>
#include "plateform.h"

#include "plateform_openglES_web.cpp"
//TODO(mtn5): remove map and implement one?
#include <unordered_map>
#include <string>

namespace TE { namespace Core { 

namespace Plateform {


std::unordered_map<std::string, keycode> JSCode2Plateform = {
    // Functions
    {"F1", F1},
    {"F2", F2},
    {"F3", F3},
    {"F4", F4},
    {"F5", F5},
    {"F6", F6},
    {"F7", F7},
    {"F8", F8},
    {"F9", F9},
    {"F10", F10},
    {"F11", F11},
    {"F12", F13},
//Specials
    {"Backspace", BACKSPACE},
    {"Tab", TAB},
    {"Enter", ENTER},
    {"Space", SPACE},
    {"CapsLock", CAPSLOCK},
    {"ControlLeft", CTL_LEFT},
    {"ControlRight", CTL_RIGHT},
    {"ShiftLeft", SHIFT_LEFT},
    {"ShiftRight", SHIFT_RIGHT},
    {"AltRight", ALT_RIGHT},
    {"AltLeft", ALT_LEFT},
// Specials
    {"End", END},
    {"Home", HOME},
    {"Insert", INSERT},
    {"Delete", DEL},
    {"PageDown", PAGE_DOWN},
    {"PageUp", PAGE_UP},
// Arrows
    {"ArrowLeft", ARROW_LEFT},
    {"ArrowRight", ARROW_RIGHT},
    {"ArrowDown", ARROW_DOWN},
    {"ArrowUp", ARROW_UP},
// Numpad
    {"Numpad0", NUMPAD_0},
    {"Numpad1", NUMPAD_1},
    {"Numpad2", NUMPAD_2},
    {"Numpad3", NUMPAD_3},
    {"Numpad4", NUMPAD_4},
    {"Numpad5", NUMPAD_5},
    {"Numpad6", NUMPAD_6},
    {"Numpad7", NUMPAD_7},
    {"Numpad8", NUMPAD_8},
    {"Numpad9", NUMPAD_9},
    {"NumpadSubstract", NUMPAD_MINUS},
    {"NumpadAdd", NUMPAD_PLUS},
    {"NumpadMultiply", NUMPAD_MULTIPLY},
    {"NumpadDecimal", NUMPAD_POINT},
    {"NumpadEnter", NUMPAD_ENTER},
    {"NumpadDivide", NUMPAD_DIVIDE},
    {"NumLock", NUMPAD_NUMLOCK},
    // Letters
    {"KeyA", A},
    {"KeyB", B},
    {"KeyC", C},
    {"KeyD", D},
    {"KeyE", E},
    {"KeyF", F},
    {"KeyG", G},
    {"KeyH", H},
    {"KeyI", I},
    {"KeyJ", J},
    {"KeyK", K},
    {"KeyL", L},
    {"KeyM", M},
    {"KeyN", N},
    {"KeyO", O},
    {"KeyP", P},
    {"KeyQ", Q},
    {"KeyR", R},
    {"KeyS", S},
    {"KeyT", T},
    {"KeyU", U},
    {"KeyV", V},
    {"KeyW", W},
    {"KeyX", X},
    {"KeyY", Y},
    {"KeyZ", Z},
    // Numbers
    {"Digit0", NUM_0},
    {"Digit1", NUM_1},
    {"Digit2", NUM_2},
    {"Digit3", NUM_3},
    {"Digit4", NUM_4},
    {"Digit5", NUM_5},
    {"Digit6", NUM_6},
    {"Digit7", NUM_7},
    {"Digit8", NUM_8},
    {"Digit9", NUM_9},
};

keycode
EMScriptenCode2Plateform(  const char* JSCode ) {
    #if 0
    // Mouse
      return MouseButton_Left;
      case VK_MBUTTON: return MouseButton_Middle;
      case VK_RBUTTON: return MouseButton_Right;
      case VK_XBUTTON1: return MouseButton_Extended;
      case VK_XBUTTON2: return MouseButton_Extended0;

    // Functions : F1-12
      case VK_F1: return F_1;
      case VK_F2: return F_2;
      case VK_F3: return F_3;
      case VK_F4: return F_4;
      case VK_F5: return F_5;
      case VK_F6: return F_6;
      case VK_F7: return F_7;
      case VK_F8: return F_8;
      case VK_F9: return F_9;
      case VK_F10: return F_10;
      case VK_F11: return F_11;
      case VK_F12: return F_12;

    // Numpad
       case VK_NUMPAD0: return NUMPAD_0;
       case VK_NUMPAD1: return NUMPAD_1;
       case VK_NUMPAD2: return NUMPAD_2;
       case VK_NUMPAD3: return NUMPAD_3;
       case VK_NUMPAD4: return NUMPAD_4;
       case VK_NUMPAD5: return NUMPAD_5;
       case VK_NUMPAD6: return NUMPAD_6;
       case VK_NUMPAD7: return NUMPAD_7;
       case VK_NUMPAD8: return NUMPAD_8;
       case VK_NUMPAD9: return NUMPAD_9;

       case VK_ADD : return NUMPAD_PLUS;
       case VK_SUBTRACT : return NUMPAD_MINUS;
       case VK_DIVIDE : return NUMPAD_DIVIDE;
       case VK_MULTIPLY : return NUMPAD_MULTIPLY;
       //case VK_? : return NUMPAD_ENTER;
       case VK_DECIMAL : return NUMPAD_POINT;

       case VK_NUMLOCK: return NUMPAD_NUMLOCK;

    // Sepcial keys
        case VK_RETURN : return ENTER;
        // handling of Alt seems different than the rest? VK_Code (1 << 29)
        case VK_RMENU : return ALT_RIGHT;
        case VK_RSHIFT : return SHIFT_RIGHT;
        case VK_RCONTROL : return CTL_RIGHT;
        case VK_LMENU : return ALT_LEFT;
        case VK_LSHIFT : return SHIFT_LEFT;
        case VK_LCONTROL : return CTL_LEFT;
        case VK_TAB : return TAB;
        case VK_SELECT : return SELECT;
        case VK_SNAPSHOT : return PRINT_SCREEN;
        case VK_INSERT : return INSERT;
        case VK_DELETE : return DEL;
        case VK_HOME : return HOME;
        case VK_END : return END;
        case VK_PRIOR : return PAGE_UP;
        case VK_NEXT : return PAGE_DOWN;
        case VK_SPACE : return SPACE;
        case VK_BACK : return BACKSPACE;
        case VK_ESCAPE : return ESCAPE;
        case VK_CAPITAL : return CAPSLOCK;

        case VK_LEFT : return ARROW_LEFT;
        case VK_RIGHT : return ARROW_RIGHT;
        case VK_UP : return ARROW_UP;
        case VK_DOWN : return ARROW_DOWN;
    
    // Numbers
        case '0': return NUM_0;
        case '1': return NUM_1;
        case '2': return NUM_2;
        case '3': return NUM_3;
        case '4': return NUM_4;
        case '5': return NUM_5;
        case '6': return NUM_6;
        case '7': return NUM_7;
        case '8': return NUM_8;
        case '9': return NUM_9;
    }
    #endif
    return JSCode2Plateform[JSCode];
    //return KEYCODE_COUNT;
};


EM_BOOL keyup_callback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
    input* NewInput = (input*) userData;
    //if(!NewInput) return EM_FALSE;

    std::cout << EMScriptenCode2Plateform( keyEvent->code ) << std::endl;
    //NewInput.Controllers[0].Keys[ EMScriptenCode2Plateform(keyEvent) ].Down = false;
    return EM_TRUE;
}

EM_BOOL keydown_callback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
    input* NewInput = (input*) userData;
    //if(!NewInput) return EM_FALSE;

    std::cout << keyEvent->key <<" " << keyEvent->code << std::endl;
    //NewInput.Controllers[0].Keys[ EMScriptenCode2Plateform(keyEvent) ].Down = false;
    return EM_TRUE;
}

}
    
namespace Window { 

struct window {
    EGLDisplay Handle;
};

void Init( EGLDisplay& Display, const char* Name, const char* Title) {
    // TODO(toffa): can we name the window?
    // Name and Title unused for now on web

    Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if ( Display == EGL_NO_DISPLAY )
   {
	   std::cout << "ERROR: make display\n" << std::endl;
      return ;
   }
	
	EGLint Major,Minor;
	if( !eglInitialize(Display, &Minor, &Major) ) {
		std::cout << "ERROR: make init\n" << std::endl;
		return ;
	}

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, Plateform::keydown_callback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, Plateform::keyup_callback);
}

void Show(EGLDisplay& Display) {
    // TODO(toffa): is it implementable?
   // Not implemented for web as window is always shown? 
}

}


}
}
