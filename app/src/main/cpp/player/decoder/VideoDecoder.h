//
// Created by lyhao on 2023/10/31.
//

#ifndef XVIDEOPLAYER_VIDEODECODER_H
#define XVIDEOPLAYER_VIDEODECODER_H
#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavutil/avutil.h>
};
#include "DecoderBase.h"

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
private:
    virtual void OnDecoderReady();
    virtual void OnFrameAvailable(AVFrame *avFrame);
    int m_VideoWidth = 0;
    int m_VideoHeight = 0;
};


#endif //XVIDEOPLAYER_VIDEODECODER_H
