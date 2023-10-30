//
// Created by lyhao on 2023/10/24.
//

#ifndef XVIDEOPLAYER_PLAYERWRAPPER_H
#define XVIDEOPLAYER_PLAYERWRAPPER_H

#include <jni.h>
#include "FFMediaPlayer.h"

static const int FFMEDIA_PLAYER = 0;
static const int HWCODEC_PLAYER = 1;

class PlayerWrapper {
public:
    PlayerWrapper(){};
    virtual ~PlayerWrapper(){};

    void Init(JNIEnv *jniEnv, jobject obj, char *url, int playerType, int renderType, jobject surface);
    void UnInit();

    void Play();
    void Pause();
    void Stop();
private:
    MediaPlayer *m_MediaPlayer = nullptr;
};


#endif //XVIDEOPLAYER_PLAYERWRAPPER_H
