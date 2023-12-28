//
// Created by lyhao on 2023/10/31.
//

#ifndef XVIDEOPLAYER_VIDEODECODER_H
#define XVIDEOPLAYER_VIDEODECODER_H
#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavutil/avutil.h>
#include <libavutil//imgutils.h>
#include <libswscale//swscale.h>
};
#include "DecoderBase.h"
#include "VideoRender.h"

class VideoDecoder : public DecoderBase{
public:
    VideoDecoder(char *url) {
        Init(url, AVMEDIA_TYPE_VIDEO);
    }
    virtual ~VideoDecoder(){
        UnInit();
    }

    int GetVideoWidth() {
        return m_VideoWidth;
    }
    int GetVideoHeight() {
        return m_VideoHeight;
    }
    float GetDuration() {
        return (float) m_Duration / 1000.0f;
    }

    void SetVideoRender(VideoRender *videoRender){
        m_VideoRender = videoRender;
    }

private:
    virtual void OnDecoderReady();
    virtual void OnDecoderDone();
    virtual void OnFrameAvailable(AVFrame *avFrame);

    VideoRender *m_VideoRender = nullptr;
    int m_VideoWidth = 0;
    int m_VideoHeight = 0;

    int m_RenderWidth = 0;
    int m_RenderHeight = 0;

    AVFrame *m_RGBAFrame = nullptr;
    uint8_t *m_FrameBuffer = nullptr;
    SwsContext *m_SwsContext = nullptr;
};


#endif //XVIDEOPLAYER_VIDEODECODER_H
