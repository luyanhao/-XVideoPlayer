//
// Created by lyhao on 2023/11/10.
//

#include <android/native_window_jni.h>
#include "NativeRender.h"
#include "LogUtil.h"

NativeRender::NativeRender(int type, JNIEnv *env, jobject surface) : VideoRender(type) {
    m_ANativeWindow = ANativeWindow_fromSurface(env, surface);
}

NativeRender::~NativeRender() {
    if (m_ANativeWindow) {
        ANativeWindow_release(m_ANativeWindow);
    }
}

void NativeRender::Init(int videoWidth, int videoHeight, int *dstSize) {
    LOGCATE("NativeRender::Init m_ANativeWindow=%p, width=%d, height=%d", m_ANativeWindow, videoWidth, videoHeight);
    if (m_ANativeWindow == nullptr) return;
    int windowWidth = ANativeWindow_getWidth(m_ANativeWindow);
    int windowHeight = ANativeWindow_getHeight(m_ANativeWindow);

    if (windowWidth < windowHeight * videoWidth / videoHeight) {
        m_DstWidth = windowWidth;
        m_DstHeight = windowWidth * videoHeight / videoWidth;
    } else {
        m_DstWidth = windowHeight * videoWidth / videoHeight;
        m_DstHeight = windowHeight;
    }
    LOGCATE("NativeRender::Init window[%d - %d], dstSize[%d - %d]", windowWidth, windowHeight, m_DstWidth, m_DstHeight);

    ANativeWindow_setBuffersGeometry(m_ANativeWindow, m_DstWidth, m_DstHeight, WINDOW_FORMAT_RGBA_8888);

    dstSize[0] = m_DstWidth;
    dstSize[1] = m_DstHeight;
}

void NativeRender::RenderVideoFrame(NativeImage *pImage) {
    if (m_ANativeWindow == nullptr || pImage == nullptr) {
        LOGCATE("NativeRender::RenderVideoFrame m_ANativeWindow=%p, pImage=%p", m_ANativeWindow, pImage);
        return;
    }
    ANativeWindow_lock(m_ANativeWindow, &m_ANativeWindow_Buffer, nullptr);
    uint8_t *dstBuffer = static_cast<uint8_t *>(m_ANativeWindow_Buffer.bits);

    int srcLineSize = pImage->width * 4; // RGBA
    int dstLineSize = m_ANativeWindow_Buffer.stride * 4;

    for (int i = 0; i < m_DstHeight; ++i) {
        memcpy(dstBuffer + i * dstLineSize, pImage->ppPlane[0] + i * srcLineSize, srcLineSize);
    }
    ANativeWindow_unlockAndPost(m_ANativeWindow);
}

void NativeRender::UnInit() {
    LOGCATD("NativeRender::UnInit");
}
