#include <EGL/egl.h>
#include <iostream>

#include "plateform_android.h"

//TODO(mtn5): remove map and implement one?
#include <unordered_map>
#include <string>

#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "plateform_openglES_android.cpp"

AAssetManager* gAssetManager = nullptr;

// NOT GOOD
/*
extern "C" {
JNIEXPORT void JNICALL Java_AndroidFileSystem_LoadAssetManager(JNIEnv *env, jclass type, jobject mgr);
}

JNIEXPORT void JNICALL Java_AndroidFileSystem_LoadAssetManager(JNIEnv *env, jclass type, jobject mgr) {
      LOGI("------------LOADING ASSET MANAGER------------");
      gAssetManager = AAssetManager_fromJava(env, mgr);
}*/

namespace TE { 
namespace Core { 
namespace Plateform {

namespace {

void handle_cmd(android_app *pApp, int32_t cmd) {
  Renderer::renderer* engine = (Renderer::renderer*) pApp->userData;	
  switch (cmd) {
  case APP_CMD_INIT_WINDOW: {
     Renderer::InitEGL(*engine);
     gIsWindowAvailable = true;
  } break;

  case APP_CMD_TERM_WINDOW:
    // TODO :: toffa: clenaup
    break;
  }
}

/**
 * Process the next input event.
 */
int32_t handle_inputs(struct android_app *app, AInputEvent *event) {
  return 0;
}

} // namespace

void MainLoop( engine& Engine) {
  int events;
  android_poll_source *pSource;
  do {
    if (ALooper_pollAll(0, nullptr, &events, (void **)&pSource) >= 0) {
      if (pSource) {
        pSource->process(Engine.Plateform->App, pSource);
      }
    }

    if(Engine.Renderer->Surface != EGL_NO_SURFACE) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(Engine.ShaderProgram);
        glBindVertexArray(Engine.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
		
	      eglSwapBuffers(Engine.Renderer->Display, Engine.Renderer->Surface);
    }
  } while (!Engine.Plateform->App->destroyRequested);
}

void Init(plateform& P) {
  P.App->onAppCmd = handle_cmd;
  P.App->onInputEvent = handle_inputs;
  Window::Init(P.Window, "MainWindow", "SuperDuper title");
  Audio::Init(P.AudioDevice);
  gAssetManager = P.App->activity->assetManager;
}

void Exit(int Error) {}

namespace Files {
    void Create(file&) {}
    void Open(file& File, const char* Path) {
      if (gAssetManager == nullptr) LOGW("NO ASSET MANAGER!!!!!!!!!");
      AAsset* asset = AAssetManager_open(gAssetManager, Path, AASSET_MODE_UNKNOWN);
      // the asset might not be found
      if (NULL == asset) {
          LOGW("Cannot Open file !!");
          return;
      }
      File.Asset = asset;
     }
    void Close(file& File) {
      AAsset_close(File.Asset);
    }
    void ReadAll(file& File, char*& Buffer, size_t& BufferSize) {
      // open asset as file descriptor
      if(File.Asset == nullptr) LOGW("Asset no loaded !!");
      Buffer = (char*)AAsset_getBuffer(File.Asset);
      BufferSize = AAsset_getLength(File.Asset);
    }
} // namespace File
} // namespace Plateform

namespace Audio {

  namespace {
    void PlayerCallback(SLAndroidSimpleBufferQueueItf self , void* Context) {
      buffer* Buffer = (buffer*)Context;
      
      LOGW("PlayerCallback");

      SLresult Err = (*self)->Enqueue(self, Buffer->data, Buffer->size);
      if(Err != SL_RESULT_SUCCESS) return;

    }
  } // namespace
    void Init(device& Device) { 
       SLObjectItf engineObject;
       SLresult Err = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
       if(Err != SL_RESULT_SUCCESS) return;
        LOGW("Create Engine");

       Err = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
       if(Err != SL_RESULT_SUCCESS) return;
        LOGW("Realize Engine");

       SLEngineItf engineEngine;
       Err = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
       if(Err != SL_RESULT_SUCCESS) return;
        LOGW("QueryItf ENGINE");

       // create output mix, with environmental reverb specified as a non-required interface
       const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
       const SLboolean req[1] = {SL_BOOLEAN_FALSE};
       SLObjectItf outputMixObject;
       Err = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
       if(Err != SL_RESULT_SUCCESS) return;
        LOGW("Create OutputMix");

       // realize the output mix
       Err = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
       if(Err != SL_RESULT_SUCCESS) return;
        LOGW("realize OutputMix");

       Device.Handle = engineEngine;
       Device.outputMixObject = outputMixObject;
    }
    void CreateBuffer(const device& Device, buffer& Buffer) { 
       SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};

       SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 
                                        2, 
                                        SL_SAMPLINGRATE_44_1,
                                        SL_PCMSAMPLEFORMAT_FIXED_16, 
                                        SL_PCMSAMPLEFORMAT_FIXED_16,
                                        SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, 
                                        SL_BYTEORDER_LITTLEENDIAN };

       SLDataSource audioSrc = {&loc_bufq, &format_pcm};

       // configure audio sink
       SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, Device.outputMixObject};
       SLDataSink audioSink = {&loc_outmix, NULL};

       const SLInterfaceID ids[2] = {SL_IID_BUFFERQUEUE};
       const SLboolean req[2] = {SL_BOOLEAN_TRUE};
       SLObjectItf playerObject;
       SLresult Err = (*Device.Handle)->CreateAudioPlayer(Device.Handle, &playerObject, &audioSrc, &audioSink, 1,ids,req);
       if(Err != SL_RESULT_SUCCESS) return;
        LOGW("Create audio player");

       Err = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
       if(Err != SL_RESULT_SUCCESS) return;
       LOGW("Relaize Audio player");
       // get the buffer queue interface
       SLAndroidSimpleBufferQueueItf PlayerBufferQueue;
       Err = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &PlayerBufferQueue);
       if(Err != SL_RESULT_SUCCESS) return;
       LOGW("QueryItf BUFFERQUEUE");

      // register callback on the buffer queue
      Err = (*PlayerBufferQueue)->RegisterCallback(PlayerBufferQueue, PlayerCallback, &Buffer);
      if(Err != SL_RESULT_SUCCESS) return;
      LOGW("RegisterCallback");

      Buffer.playerObject = playerObject;
    }                   
    void UpdateStaticBuffer(buffer& Buffer, char* Data, size_t DataSize) {
      // OnAndroid we are callback based, so we simply store data inside the buffer
      // and we serve it when we are called at the end of the buffer
      Buffer.data = Data;
      Buffer.size = DataSize;

      LOGW("DATASIZE %zu", DataSize);
     }
    void Play(const buffer& Buffer) {
       // get the play interface
       LOGW("PLAY SOUnd");
       // Load first buffer
       SLAndroidSimpleBufferQueueItf BufferQueue;
       (*Buffer.playerObject)->GetInterface(Buffer.playerObject, SL_IID_BUFFERQUEUE, &BufferQueue);
       PlayerCallback( BufferQueue, (void*)&Buffer );

       SLPlayItf playerPlay;
       SLresult Err = (*Buffer.playerObject)->GetInterface(Buffer.playerObject, SL_IID_PLAY, &playerPlay);
       if(Err != SL_RESULT_SUCCESS) return;
        // set the player's state to playing
       Err = (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
       if(Err != SL_RESULT_SUCCESS) return;
       LOGW("PLAY SOUND END");
     }
} // namespace Audio
    
} // namespace Core
} // namespace TE
