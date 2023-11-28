//
// Created by lyhao on 2023/11/28.
//

#ifndef XVIDEOPLAYER_AUDIODECODER_H
#define XVIDEOPLAYER_AUDIODECODER_H

extern "C" {
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
};
#include "DecoderBase.h"
#include "Decoder.h"

class AudioDecoder : public DecoderBase{
public:
    AudioDecoder(char *url) {
        DecoderBase::Init(url, AVMEDIA_TYPE_AUDIO);
    }
    ~AudioDecoder(){
        DecoderBase::UnInit();
    }

private:
    virtual void OnDecoderReady();
    virtual void OnDecoderDone();
    virtual void OnFrameAvailable(AVFrame *frame);
    virtual void ClearCache();

};


#endif //XVIDEOPLAYER_AUDIODECODER_H
