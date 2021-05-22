#pragma once

#ifdef _TE_WIN32_
#include "plateform_win32.h"
#endif

#ifdef __EMSCRIPTEN__
#include "plateform_web.h"
#endif

#ifdef __ANDROID__
#include "plateform_android.h"
#endif


namespace TE {namespace Core {
    struct engine;
    namespace Audio {struct device;}
    namespace Renderer {struct renderer;}
    namespace Plateform{struct plateform;
        namespace Files{struct file;}
    }}}

void te_main_entry(TE::Core::Plateform::plateform& );

namespace TE { 
namespace Core { 
namespace Plateform {

void Exit(int);
void MainLoop(engine&);

namespace Files {
    void Create(file&, const char*);
    void Open(file&, const char*);
    void Close(file&);
    void ReadAll(file&, char*&, size_t&);
} // namespace Files

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

void Init(plateform&);

} // namespace Plateform

namespace Renderer {
    void Init(renderer&);
} // namespace Renderer

namespace Audio {
    void Init(device&);
    void CreateBuffer(const device&, buffer&);
    void UpdateStaticBuffer(buffer&, char*, size_t);
    void Play(const buffer&);
} // namespace Audio


// TODO(toffa): remove this and put it inside the real game file
struct engine {
    Plateform::plateform* Plateform;
    Renderer::renderer* Renderer;

// TODO(toffa): remove me!
    unsigned int ShaderProgram;
    unsigned int VAO;
};

} // namespace Core
} // namespace TE

