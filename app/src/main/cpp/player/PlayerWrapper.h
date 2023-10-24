//
// Created by lyhao on 2023/10/24.
//

#ifndef XVIDEOPLAYER_PLAYERWRAPPER_H
#define XVIDEOPLAYER_PLAYERWRAPPER_H

#include <jni.h>

class PlayerWrapper {
public:
    PlayerWrapper(){};
    virtual ~PlayerWrapper(){};

    void Init(JNIEnv *jniEnv, jobject obj, char *url, int playerType, int renderType, jobject surface);
    void UnInit();

    void Play();
    void Pause();
    void Stop();
};


#endif //XVIDEOPLAYER_PLAYERWRAPPER_H
