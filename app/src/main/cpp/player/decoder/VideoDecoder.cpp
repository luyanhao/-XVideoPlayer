//
// Created by lyhao on 2023/10/31.
//

#include "VideoDecoder.h"
#include "LogUtil.h"

void VideoDecoder::OnDecoderReady() {
    m_VideoHeight = GetCodeContext()->height;
    m_VideoWidth = GetCodeContext()->width;

    if (m_MsgContext && m_MessageCallback) {
        m_MessageCallback(m_MsgContext, MSG_DECODER_READY, 0);
    }
    if (m_VideoRender != nullptr) {
        int dstSize[2] = {0};
        m_VideoRender->Init(m_VideoWidth, m_VideoHeight, dstSize);

        m_RenderWidth = dstSize[0];
        m_RenderHeight = dstSize[1];
        if (m_VideoRender->GetRenderType() == VIDEO_RENDER_ANWINDOW) {
            m_RGBAFrame = av_frame_alloc();
            int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_RenderWidth, m_RenderHeight, 1);
            m_FrameBuffer = (uint8_t *) av_malloc(bufferSize * sizeof(uint8_t));
            av_image_fill_arrays(m_RGBAFrame->data, m_RGBAFrame->linesize,
                                 m_FrameBuffer, AV_PIX_FMT_RGBA, m_RenderWidth, m_RenderHeight, 1);

            m_SwsContext = sws_getContext(m_VideoWidth, m_VideoHeight, GetCodeContext()->pix_fmt,
                                          m_RenderWidth, m_RenderHeight, AV_PIX_FMT_RGBA,
                                          SWS_FAST_BILINEAR, NULL, NULL, NULL);
        }
    } else {
        LOGCATE("VideoDecoder::OnDecoderReady m_VideoRender = null");
    }
}

void VideoDecoder::OnFrameAvailable(AVFrame *avFrame) {
    if (m_VideoRender != nullptr && avFrame != nullptr) {
        NativeImage image;
        if (m_VideoRender->GetRenderType() == VIDEO_RENDER_ANWINDOW) {
//            LOGCATE("VideoDecoder::OnDecoderReady %d ~ %d, %d, %d", avFrame->width, avFrame->height, avFrame->coded_picture_number, avFrame->pict_type);
            sws_scale(m_SwsContext, avFrame->data, avFrame->linesize, 0,
                      m_VideoHeight, m_RGBAFrame->data, m_RGBAFrame->linesize);

            image.format = IMAGE_FORMAT_RGBA;
            image.width = m_RenderWidth;
            image.height = m_RenderHeight;
            image.ppPlane[0] = m_RGBAFrame->data[0];
            image.pLineSize[0] = image.width * 4;
        }

        m_VideoRender->RenderVideoFrame(&image);
    }

    //TODO 先屏蔽
//    if(m_MsgContext && m_MessageCallback) {
//        m_MessageCallback(m_MsgContext, MSG_REQUEST_RENDER, 0);
//    }
}
