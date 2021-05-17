#pragma once

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
};

enum controller_type {
    Keyboard,
    Gamepad,
    
    ControllerType_Count,
};

struct controller {
    controller_type Type;
    
    
};

enum game_input_mouse_button
    {
        
        PlatformMouseButton_Count,
    };

constexpr unsigned int MAX_CONTROLLERS_COUNT = 4;
game_input {
    double DeltaTime;

    controller Controllers[MAX_CONTROLLERS_COUNT];
    mouse Mouse[PlatformMouseButton_Count];

    bool ShiftDown, AltDown, CtlDown;

}
