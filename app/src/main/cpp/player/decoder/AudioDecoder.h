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
#include "AudioRender.h"
// 音频编码采样率
static const int AUDIO_DST_SAMPLE_RATE = 48000;
// 音频编码通道数
static const int AUDIO_DST_CHANNEL_COUNTS = 2;
// 音频编码声道格式
static const uint64_t AUDIO_DST_CHANNEL_LAYOUT = AV_CH_LAYOUT_STEREO;
// 音频编码比特率
static const int AUDIO_DST_BIT_RATE = 64000;
// ACC音频一帧采样数
static const int ACC_NB_SAMPLES = 1024;

class AudioDecoder : public DecoderBase{
public:
    AudioDecoder(char *url) {
        Init(url, AVMEDIA_TYPE_AUDIO);
    }
    ~AudioDecoder(){
        UnInit();
    }

    void SetAudioRender(AudioRender *mAudioRender);

private:
    virtual void OnDecoderReady();
    virtual void OnDecoderDone();
    virtual void OnFrameAvailable(AVFrame *frame);
    virtual void ClearCache();

    const AVSampleFormat DST_SAMPLT_FORMAT = AV_SAMPLE_FMT_S16;

    AudioRender  *m_AudioRender = nullptr;

    //audio resample context
    SwrContext   *m_SwrContext = nullptr;

    uint8_t      *m_AudioOutBuffer = nullptr;

    //number of sample per channel
    int           m_nbSamples = 0;

    //dst frame data size
    int           m_DstFrameDataSze = 0;
};


#endif //XVIDEOPLAYER_AUDIODECODER_H
