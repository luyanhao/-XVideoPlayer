//
// Created by lyhao on 2023/10/24.
//

#include "FFMediaPlayer.h"

void FFMediaPlayer::Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) {
    m_VideoDecoder = new VideoDecoder(url);
}

void FFMediaPlayer::UnInit() {

}

void FFMediaPlayer::Play() {

}

void FFMediaPlayer::Pause() {

}

void FFMediaPlayer::Stop() {

}

void FFMediaPlayer::SeekToPosition(float position) {

}

long FFMediaPlayer::GetMediaParams(int paramType) {
    return 0;
}
