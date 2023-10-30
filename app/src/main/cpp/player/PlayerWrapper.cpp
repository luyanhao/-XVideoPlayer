//
// Created by lyhao on 2023/10/24.
//

#include "PlayerWrapper.h"

void PlayerWrapper::Init(JNIEnv *jniEnv, jobject obj, char *url, int playerType, int renderType,
                         jobject surface) {
    switch (playerType) {
        case FFMEDIA_PLAYER:
            m_MediaPlayer = new FFMediaPlayer();
            break;
        case HWCODEC_PLAYER:
            break;
        default:
            break;
    }
}

void PlayerWrapper::UnInit() {
    if(m_MediaPlayer) {
        m_MediaPlayer->UnInit();
        delete m_MediaPlayer;
        m_MediaPlayer = nullptr;
    }
}

void PlayerWrapper::Play() {
    if(m_MediaPlayer) {
        m_MediaPlayer->Play();
    }
}

void PlayerWrapper::Pause() {
    if(m_MediaPlayer) {
        m_MediaPlayer->Pause();
    }
}

void PlayerWrapper::Stop() {
    if(m_MediaPlayer) {
        m_MediaPlayer->Stop();
    }
}
