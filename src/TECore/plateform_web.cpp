#include "plateform_web.h"

#include <iostream>
#include <stdio.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include "plateform_openglES_web.cpp"
//TODO(mtn5): remove map and implement one?
#include <unordered_map>
#include <string>

namespace TE { 
namespace Core { 
namespace Plateform {

namespace {
std::unordered_map<std::string, keycode> JSCode2Plateform = {
    // Functions
    {"F1", F_1},
    {"F2", F_2},
    {"F3", F_3},
    {"F4", F_4},
    {"F5", F_5},
    {"F6", F_6},
    {"F7", F_7},
    {"F8", F_8},
    {"F9", F_9},
    {"F10", F_10},
    {"F11", F_11},
    {"F12", F_12},
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
    // NOTe(toffa):potentially dangerous to return without checking value...
    return JSCode2Plateform[JSCode];
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

} // namespace


void MainLoop( engine& Engine ) {
    emscripten_set_main_loop_arg( mainLoop, &Engine, 0, true );
}

void Init(plateform& Plateform) {
    Window::Init(Plateform.Window, "MainWindow", "SuperDuper title");
    Audio::Init(Plateform.AudioDevice);
}

void Exit(int Error) {}

namespace Files {
    void Create(file&) {}
    void Open(file& File, const char* Path) {
        File.Handle = fopen(Path, "r");
     }
    void Close(file& File) {
        fclose(File.Handle);
    }
    void ReadAll(file& File, char*& Buffer, size_t& BufferSize) {
        fseek(File.Handle, 0, SEEK_END);
        BufferSize = ftell(File.Handle);
        fseek(File.Handle, 0, SEEK_SET);  /* same as rewind(f); */
        Buffer = (char*)malloc( (BufferSize + 1) * sizeof(char));
        fread(Buffer, 1, BufferSize, File.Handle);
     }
} // namespace File
} // namespace Plateform

namespace Audio {
    void Init(device& Device) {
        Device.AudioContext = val::global("AudioContext");
        if (!Device.AudioContext.as<bool>()) {
          Device.AudioContext = val::global("webkitAudioContext");
        }
        val V = val::object();
        V.set("sampleRate", 44100);
        Device.Context = Device.AudioContext.new_(V);
     }
    void CreateBuffer(const device& Device, buffer& Buffer) {
            // Create an empty three-second stereo buffer at the sample rate of the AudioContext
            Buffer.Handle = Device.Context.call<val>("createBuffer", 2, Device.Context["sampleRate"].as<int>() * 4, Device.Context["sampleRate"].as<int>());
            Buffer.Context = Device.Context;
     }                   
    void UpdateStaticBuffer(buffer& Buffer, char* Data, size_t DataSize) {
            // Assume that we are always getting back PCM LittleEndian 16bits Interleaved 44.1 2Ch
            // TODO (toffa): Actually decode WAV RIFF Header? Honestly we will probably compress the audio
            // ourselves, so we are probably safe to hard encode the input stream constraints
            int chunkSize = Data[40] | Data[41] << 8 | Data[42] << 16 | Data[43] << 24;
            int chunkOffset = 44;
            for (int channel = 0; channel < Buffer.Handle["numberOfChannels"].as<int>(); channel++) {
              // This gives us the actual ArrayBuffer that contains the data
              val nowBuffering = Buffer.Handle.call<val>("getChannelData", channel);
              for (int i = chunkOffset, j =0; i < chunkSize; i+=4, j++) {
                // audio needs to be in [-1.0; 1.0]
                // note: valmin does not carry the sign -, as it will be carried by value
                const float valMin = /*-*/32768;
                const float valMax = 32767;
                char offset = (channel*2);
                float PCM16 = Data[i + offset ] | (Data[ (i+1) + offset ] << 8);
                float Value = PCM16 / ((PCM16<0)?valMin:valMax);
                if( j < nowBuffering["length"].as<int>() )
                    nowBuffering.set(std::to_string(j), val(Value));
              }
            }
            // Get an AudioBufferSourceNode.
            // This is the AudioNode to use when we want to play an AudioBuffer
            Buffer.Source = Buffer.Context.call<val>("createBufferSource");
            // set the buffer in the AudioBufferSourceNode
            Buffer.Source.set("buffer", Buffer.Handle);
            // connect the AudioBufferSourceNode to the
            // destination so we can hear the sound
            Buffer.Source.call<void>("connect", Buffer.Context["destination"]);
            Buffer.Source.set("loop", val(true));
     }
    void Play(const buffer& Buffer) { 
            // start the source playing
            Buffer.Source.call<void>("start");
    }
} // namespace Audio
    
namespace Window { 

void Init( window& Window, const char* Name, const char* Title) {
    // TODO(toffa): can we name the window?
    // Name and Title unused for now on web
    EGLDisplay Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
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

    Window.Handle = Display;

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, Plateform::keydown_callback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, Plateform::keyup_callback);
}

void Show(window& Display) {
    // TODO(toffa): is it implementable?
   // Not implemented for web as window is always shown? 
}

} // namespace Window
} // namespace COre
} // namespace TE
