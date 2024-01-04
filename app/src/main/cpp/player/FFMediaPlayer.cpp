//
// Created by lyhao on 2023/10/24.
//

#include <render/video/NativeRender.h>
#include "FFMediaPlayer.h"
#include "LogUtil.h"
#include "OpenSLRender.h"
#include "VideoGLRender.h"

void FFMediaPlayer::Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) {

    jniEnv->GetJavaVM(&m_JavaVM);
    m_JavaObj = jniEnv->NewGlobalRef(obj);
    LOGCATE("FFMediaPlayer::Init------------");
    m_VideoDecoder = new VideoDecoder(url);
    m_AudioDecoder = new AudioDecoder(url);

    if (renderType == VIDEO_RENDER_ANWINDOW) {
        m_VideoRender = new NativeRender(renderType, jniEnv, surface);
        m_VideoDecoder->SetVideoRender(m_VideoRender);
    } else if (renderType == VIDEO_RENDER_OPENGL) {
        m_VideoDecoder->SetVideoRender(VideoGLRender::GetInstance());
    }

    m_AudioRender = new OpenSLRender();
    m_AudioDecoder->SetAudioRender(m_AudioRender);

    m_AudioDecoder->SetMessageCallback(this, PostMessage);
    m_VideoDecoder->SetMessageCallback(this, PostMessage);
}

void FFMediaPlayer::UnInit() {
    LOGCATE("FFMediaPlayer::UnInit");
    if(m_VideoDecoder) {
        delete m_VideoDecoder;
        m_VideoDecoder = nullptr;
    }

    if(m_VideoRender) {
        delete m_VideoRender;
        m_VideoRender = nullptr;
    }

    if(m_AudioDecoder) {
        delete m_AudioDecoder;
        m_AudioDecoder = nullptr;
    }

    if(m_AudioRender) {
        delete m_AudioRender;
        m_AudioRender = nullptr;
    }
    bool isAttach = false;
    GetJNIEnv(&isAttach)->DeleteGlobalRef(m_JavaObj);
    if (isAttach) {
        GetJavaVM()->DetachCurrentThread();
    }
    LOGCATE("FFMediaPlayer::UnInit End");
}

void FFMediaPlayer::Play() {
    if(m_VideoDecoder) {
        m_VideoDecoder->Start();
    }
    if(m_AudioDecoder) {
        m_AudioDecoder->Start();
    }
}

void FFMediaPlayer::Pause() {

}

void FFMediaPlayer::Stop() {
    if(m_VideoDecoder) {
        m_VideoDecoder->Stop();
    }
    if(m_AudioDecoder) {
        m_AudioDecoder->Stop();
    }
}

void FFMediaPlayer::SeekToPosition(float position) {
    if(m_VideoDecoder) {
        m_VideoDecoder->SeekToPosition(position);
    }
    if(m_AudioDecoder) {
        m_AudioDecoder->SeekToPosition(position);
    }
}

long FFMediaPlayer::GetMediaParams(int paramType) {
    switch (paramType) {
        case MEDIA_PARAM_VIDEO_WIDTH: {
            return m_VideoDecoder != nullptr ? m_VideoDecoder->GetVideoWidth() : 0;
        }
        case MEDIA_PARAM_VIDEO_HEIGHT: {
            return m_VideoDecoder != nullptr ? m_VideoDecoder->GetVideoHeight() : 0;
        }
        case MEDIA_PARAM_VIDEO_DURATION: {
            return m_VideoDecoder != nullptr ? m_VideoDecoder->GetDuration() : 0;
        }
    }
    return 0;
}


JNIEnv *FFMediaPlayer::GetJNIEnv(bool *isAttach) {
    if (m_JavaVM == nullptr) {
        LOGCATE("FFMediaPlayer::GetJNIEnv m_JavaVM == nullptr");
        return nullptr;
    }
    JNIEnv *env;
    int status = m_JavaVM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4);
    if (status != JNI_OK) {
        status = m_JavaVM->AttachCurrentThread(&env, nullptr);
        if (status != JNI_OK) {
            LOGCATE("FFMediaPlayer::GetJNIEnv failed to attach current thread");
            return nullptr;
        }
        *isAttach = true;
    }
    return env;
}

jobject FFMediaPlayer::GetJavaObj() {
    return m_JavaObj;
}
JavaVM *FFMediaPlayer::GetJavaVM() {
    return m_JavaVM;
}

void FFMediaPlayer::PostMessage(void *context, int msgType, float msgValue) {
    if (context != nullptr) {
        FFMediaPlayer *player = static_cast<FFMediaPlayer *>(context);
        bool isAttach = false;
        JNIEnv *env = player->GetJNIEnv(&isAttach);
//        LOGCATE("FFMediaPlayer::PostMessage env=%p", env);
        if (env == nullptr)
            return;
        jobject javaObj = player->GetJavaObj();
        jmethodID mid = env->GetMethodID(env->GetObjectClass(javaObj), JAVA_PLAYER_EVENT_CALLBACK_API_NAME, "(IF)V");
        env->CallVoidMethod(javaObj, mid, msgType, msgValue);
        if (isAttach) {
            player->GetJavaVM()->DetachCurrentThread(); // TODO 这里还需要进一步学习
        }
    }
}
