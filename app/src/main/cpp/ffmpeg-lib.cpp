//
// Created by lyhao on 2023/10/17.
//
#include <cstdio>
#include <cstring>
#include "jni.h"
#include "util/LogUtil.h"

#include "PlayerWrapper.h"

extern "C" {
#include <libavcodec/version.h>
#include <libavcodec/avcodec.h>
#include <libavformat/version.h>
#include <libavutil/version.h>
#include <libavfilter/version.h>
#include <libswresample/version.h>
#include <libswscale/version.h>
};

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_byteflow_learnffmpeg_media_FFMediaPlayer
 * Method:    nativeGetFFmpegVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_lyhao_xvideoplayer_media_FFMediaPlayer_nativeGetFFmpegVersion
        (JNIEnv *env, jclass cls) {
    char strBuffer[1024 * 4] = {0};
    strcat(strBuffer, "libavcodec : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVCODEC_VERSION));
    strcat(strBuffer, "\nlibavformat : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFORMAT_VERSION));
    strcat(strBuffer, "\nlibavutil : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVUTIL_VERSION));
    strcat(strBuffer, "\nlibavfilter : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFILTER_VERSION));
    strcat(strBuffer, "\nlibswresample : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWRESAMPLE_VERSION));
    strcat(strBuffer, "\nlibswscale : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWSCALE_VERSION));
    strcat(strBuffer, "\navcodec_configure : \n");
    strcat(strBuffer, avcodec_configuration());
    strcat(strBuffer, "\navcodec_license : ");
    strcat(strBuffer, avcodec_license());
    LOGCATE("GetFFmpegVersion\n%s", strBuffer);
    //ASanTestCase::MainTest();
    return env->NewStringUTF(strBuffer);
}

JNIEXPORT jlong JNICALL Java_com_lyhao_xvideoplayer_media_FFMediaPlayer_nativeInit
        (JNIEnv *env, jobject obj, jstring jurl, int playerType, int renderType, jobject surface) {
    const char* url = env->GetStringUTFChars(jurl, nullptr);
    PlayerWrapper* player = new PlayerWrapper();
    player->Init(env, obj, const_cast<char *>(url), playerType, renderType, surface);
    env->ReleaseStringUTFChars(jurl, url);
    return reinterpret_cast<jlong>(player);
}
JNIEXPORT void JNICALL Java_com_lyhao_xvideoplayer_media_FFMediaPlayer_nativePlay
        (JNIEnv *env, jobject obj, jlong playerHandler) {
    if(playerHandler != 0) {
        PlayerWrapper *playerWrapper = reinterpret_cast<PlayerWrapper *>(playerHandler);
        playerWrapper->Play();
    }
}

JNIEXPORT jlong JNICALL
Java_com_lyhao_xvideoplayer_media_FFMediaPlayer_nativeGetMediaParams(JNIEnv *env, jclass clazz,
                                                                     jlong playerHandler, jint param_type) {
    if(playerHandler != 0) {
        PlayerWrapper *playerWrapper = reinterpret_cast<PlayerWrapper *>(playerHandler);
        return playerWrapper->GetMediaParams(param_type);
    }
    return 0;
}

JNIEXPORT void JNICALL
Java_com_lyhao_xvideoplayer_media_FFMediaPlayer_nativeSeekToPosition(JNIEnv *env, jobject clazz, jlong playerHandler, jfloat position) {
    if(playerHandler != 0) {
        PlayerWrapper *playerWrapper = reinterpret_cast<PlayerWrapper *>(playerHandler);
        playerWrapper->SeekToPosition(position);
    }
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGCATE("JNI_OnLoad");
    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif