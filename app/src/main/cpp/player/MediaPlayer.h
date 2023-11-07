//
// Created by lyhao on 2023/10/24.
//

#ifndef XVIDEOPLAYER_MEDIAPLAYER_H
#define XVIDEOPLAYER_MEDIAPLAYER_H

#define JAVA_PLAYER_EVENT_CALLBACK_API_NAME "playerEventCallback"

#include <jni.h>
#include <decoder/VideoDecoder.h>

class MediaPlayer{
public:
    MediaPlayer(){};
    virtual ~MediaPlayer(){};
    virtual void Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) = 0;
    virtual void UnInit() = 0;
    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;

    JavaVM  *m_JavaVM = nullptr;
    jobject m_JavaObj = nullptr;
};

#endif //XVIDEOPLAYER_MEDIAPLAYER_H
