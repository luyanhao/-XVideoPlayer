//
// Created by lyhao on 2023/10/24.
//

#ifndef XVIDEOPLAYER_FFMEDIAPLAYER_H
#define XVIDEOPLAYER_FFMEDIAPLAYER_H

#include "MediaPlayer.h"

class FFMediaPlayer : public MediaPlayer {
public:
    FFMediaPlayer(){};
    virtual ~FFMediaPlayer(){};

    virtual void Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) ;
    virtual void UnInit();

    virtual void Play();
    virtual void Pause();
    virtual void Stop();
    virtual void SeekToPosition(float position);
    virtual long GetMediaParams(int paramType);
};


#endif //XVIDEOPLAYER_FFMEDIAPLAYER_H
