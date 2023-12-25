//
// Created by lyhao on 2023/11/28.
//

#ifndef XVIDEOPLAYER_OPENSLRENDER_H
#define XVIDEOPLAYER_OPENSLRENDER_H

#include <SLES/OpenSLES.h>
#include <SLES//OpenSLES_Android.h>
#include <queue>
#include <thread>
#include "AudioRender.h"

#define MAX_QUEUE_BUFFER_SIZE 3

class OpenSLRender : public AudioRender{
public:
    OpenSLRender(){}
    virtual ~OpenSLRender(){}
    void Init();
    void ClearAudioCache();
    void RenderAudioFrame(uint8_t *pData, int dataSize) ;
    void UnInit();

private:
    int CreateEngine();
    int CreateOutputMixer();
    int CreateAudioPlayer();
    int GetAudioFrameQueueSize();
    void StartRender();
    void HandleAudioFrameQueue();
    static void CreateSLWaitingThread(OpenSLRender *openSlRender);
    static void AudioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueueItf, void *context);

    SLObjectItf m_EngineObj = nullptr;
    SLEngineItf m_EngineEngine = nullptr;
    SLObjectItf m_OutputMixObj = nullptr;
    SLObjectItf m_AudioPlayerObj = nullptr;
    SLPlayItf m_AudioPlayerPlay = nullptr;
    SLVolumeItf m_AudioPlayerVolume = nullptr;
    SLAndroidSimpleBufferQueueItf m_BufferQueue;

    std::queue<AudioFrame *> m_AudioFrameQueue;

    std::thread *m_thread = nullptr;
    std::mutex m_Mutex;
    std::condition_variable m_Cond;
    volatile bool m_Exit = false;
};


#endif //XVIDEOPLAYER_OPENSLRENDER_H
