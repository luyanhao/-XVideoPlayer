//
// Created by lyhao on 2023/10/24.
//

#ifndef XVIDEOPLAYER_MEDIAPLAYER_H
#define XVIDEOPLAYER_MEDIAPLAYER_H

#define JAVA_PLAYER_EVENT_CALLBACK_API_NAME "playerEventCallback"

#define MEDIA_PARAM_VIDEO_WIDTH         0x0001
#define MEDIA_PARAM_VIDEO_HEIGHT        0x0002
#define MEDIA_PARAM_VIDEO_DURATION      0x0003

#include <jni.h>
#include <decoder/VideoDecoder.h>
#include <decoder/AudioDecoder.h>

class MediaPlayer{
public:
    MediaPlayer(){};
    virtual ~MediaPlayer(){};
    virtual void Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) = 0;
    virtual void UnInit() = 0;
    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;
    virtual long GetMediaParams(int paramType) = 0;

    JavaVM  *m_JavaVM = nullptr;
    jobject m_JavaObj = nullptr;

    virtual void SeekToPosition(jfloat d) = 0;
};

#endif //XVIDEOPLAYER_MEDIAPLAYER_H
