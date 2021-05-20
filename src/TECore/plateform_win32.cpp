#include <cstdio>
#include <windows.h>
#include <iostream>

#include "plateform.h"
#include "plateform_opengl_win32.cpp"

namespace TE { namespace Core { namespace Window { 

    struct window {
        HWND Handle;
    };



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
    case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            BeginPaint(hwnd, &Paint);
            EndPaint(hwnd, &Paint);
        } break;
	case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void Init( HWND& Window , const char* Name, const char* Title) {
	HINSTANCE hInstance = GetModuleHandle(0);	
	// Register the window class.    
    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = Name;
	wc.style = CS_OWNDC;

    RegisterClass(&wc);

    // Create the window.
    Window = CreateWindowExA(
        0,                              // Optional window styles.
        Name,                     // Window class
        Title,    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );
}

void Show(HWND Window) {
    ShowWindow(Window, SW_SHOW);
}

} // namespace Window

namespace Plateform {

keycode
VKCode2Plateform( unsigned int VKCode ) {
    switch(VKCode) {
    // Mouse
      case VK_LBUTTON: return MouseButton_Left;
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
    
    // Letters
        case 'A': return A;
        case 'B': return B;
        case 'C': return C;
        case 'D': return D;
        case 'E': return E;
        case 'F': return F;
        case 'G': return G;
        case 'H': return H;
        case 'I': return I;
        case 'J': return J;
        case 'K': return K;
        case 'L': return L;
        case 'M': return M;
        case 'N': return N;
        case 'O': return O;
        case 'P': return P;
        case 'Q': return Q;
        case 'R': return R;
        case 'S': return S;
        case 'T': return T;
        case 'U': return U;
        case 'V': return V;
        case 'W': return W;
        case 'X': return X;
        case 'Y': return Y;
        case 'Z': return Z;

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
    return KEYCODE_COUNT;
};

void GetInputs(input& Result) {
    // Get keyboard inputs (also mouse buttons)
    MSG Msg = {};
    while( PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) ) {
        switch(Msg.message)
        {
            case WM_QUIT:
            {
                gIsRunning = false;
            } break;
            
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                unsigned int VKCode = (unsigned int)Msg.wParam;
                Result.Controllers[0].Keys[ VKCode2Plateform(VKCode) ].Down = true;
                
                bool AltKeyWasDown = (Msg.lParam & (1 << 29));
                bool ShiftKeyWasDown = (GetKeyState(VK_SHIFT) & (1 << 15));
                
                // NOTE(casey): Since we are comparing WasDown to IsDown,
                // we MUST use == and != to convert these bit tests to actual
                // 0 or 1 values.
                bool WasDown = ((Msg.lParam & (1 << 30)) != 0);
                bool IsDown = ((Msg.lParam & (1UL << 31)) == 0);
                
           }
            default:
            {
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
            } break;
        }
    }
    // Get Mouse cursor
    POINT MouseP;
    GetCursorPos(&MouseP);
    // TODO(toffa): probably not a good idea to do a GetActivewindow...
    ScreenToClient(GetActiveWindow(), &MouseP);
    Result.Controllers[0].MouseX = MouseP.x;
    Result.Controllers[0].MouseY = MouseP.y;
}

}
}
}
