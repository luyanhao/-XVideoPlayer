//
// Created by lyhao on 2023/10/31.
//

#include "VideoDecoder.h"

void VideoDecoder::OnDecoderReady() {
    if (m_MsgContext && m_MessageCallback) {
        m_MessageCallback(m_MsgContext, MSG_DECODER_READY, 0);
    }
}
