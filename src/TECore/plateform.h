#pragma once

namespace TE { namespace Core { namespace Plateform {

enum keycode {
    // Mouse
        MouseButton_Left,
        MouseButton_Middle,
        MouseButton_Right,
        MouseButton_Extended,
        MouseButton_Extended0,
    // Functions : F1-12
        F_1,
        F_2,
        F_3,
        F_4,
        F_5,
        F_6,
        F_7,
        F_8,
        F_9,
        F_10,
        F_11,
        F_12,

    // Numpad
        NUMPAD_0,
        NUMPAD_1,
        NUMPAD_2,
        NUMPAD_3,
        NUMPAD_4,
        NUMPAD_5,
        NUMPAD_6,
        NUMPAD_7,
        NUMPAD_8,
        NUMPAD_9,

        NUMPAD_PLUS,
        NUMPAD_MINUS,
        NUMPAD_DIVIDE,
        NUMPAD_MULTIPLY,
        NUMPAD_ENTER,
        NUMPAD_POINT,

        NUMPAD_NUMLOCK,

    // Sepcial keys
        ENTER,
        ALT_RIGHT,
        SHIFT_RIGHT,
        CTL_RIGHT,
        ALT_LEFT,
        SHIFT_LEFT,
        CTL_LEFT,
        TAB,
        SELECT,
        PRINT_SCREEN,
        INSERT,
        DEL,
        HOME,
        END,
        PAGE_UP,
        PAGE_DOWN,
        SPACE,
        BACKSPACE,
        ESCAPE,
        CAPSLOCK,

        ARROW_LEFT,
        ARROW_RIGHT,
        ARROW_UP,
        ARROW_DOWN,
    
    // Letters
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

    // Numbers
        NUM_0,
        NUM_1,
        NUM_2,
        NUM_3,
        NUM_4,
        NUM_5,
        NUM_6,
        NUM_7,
        NUM_8,
        NUM_9,

        KEYCODE_COUNT,
};

enum controller_type {
    Keyboard,
    Gamepad,
    
    ControllerType_Count,
};

struct input_value {
    union {
        bool Down;
        bool Pressed;
    };
};

struct controller {
    controller_type Type;
    
    input_value Keys[KEYCODE_COUNT];
    
    float MouseX;
    float MouseY;
};

#define MAX_CONTROLLERS_COUNT 4
struct input {
    double DeltaTime;

    controller Controllers[ MAX_CONTROLLERS_COUNT ];
};

void GetInputs( input& Result );

}
}
}