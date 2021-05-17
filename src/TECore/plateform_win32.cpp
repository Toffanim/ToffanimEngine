#include <cstdio>
#include <windows.h>
#include <iostream>
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

}
}
}
