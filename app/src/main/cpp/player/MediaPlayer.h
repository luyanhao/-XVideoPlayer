//
// Created by lyhao on 2023/10/24.
//

#ifndef XVIDEOPLAYER_MEDIAPLAYER_H
#define XVIDEOPLAYER_MEDIAPLAYER_H

#include <jni.h>

class MediaPlayer{
public:
    MediaPlayer(){};
    virtual ~MediaPlayer(){};
    virtual void Init(JNIEnv *jniEnv, jobject obj, char *url, int playerType, int renderType, jobject surface) = 0;
    virtual void UnInit() = 0;
    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;
};

#endif //XVIDEOPLAYER_MEDIAPLAYER_H
