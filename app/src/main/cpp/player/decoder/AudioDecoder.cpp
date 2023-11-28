//
// Created by lyhao on 2023/11/28.
//

#include "AudioDecoder.h"
#include "LogUtil.h"

void AudioDecoder::OnDecoderReady() {
    LOGCATI("AudioDecoder::OnDecoderReady");
}

void AudioDecoder::OnDecoderDone() {

}

void AudioDecoder::OnFrameAvailable(AVFrame *frame) {
    LOGCATE("AudioDecoder::OnFrameAvailable frame=%p, frame->nb_samples=%d", frame, frame->nb_samples);
}

void AudioDecoder::ClearCache() {

}
