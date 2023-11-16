//
// Created by lyhao on 2023/11/10.
//

#ifndef XVIDEOPLAYER_NATIVERENDER_H
#define XVIDEOPLAYER_NATIVERENDER_H

#include <jni.h>
#include <android/native_window.h>
#include "VideoRender.h"

class NativeRender : public VideoRender{
public:
    NativeRender(int type, JNIEnv *env, jobject surface);
    virtual ~NativeRender();
    virtual void Init(int videoWidth, int videoHeight, int *dstSize);
    virtual void RenderVideoFrame(NativeImage *pImage);
    virtual void UnInit();
private:
    ANativeWindow_Buffer m_ANativeWindow_Buffer;
    ANativeWindow *m_ANativeWindow = nullptr;
    int m_DstWidth;
    int m_DstHeight;
};


#endif //XVIDEOPLAYER_NATIVERENDER_H
