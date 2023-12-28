//
// Created by lyhao on 2023/10/23.
//

#include "DecoderBase.h"
#include "LogUtil.h"

int DecoderBase::Init(const char* url, AVMediaType mediaType){
    strcpy(m_Url, url);
    m_MediaType = mediaType;
    return 0;
}

void DecoderBase::UnInit() {
    if(m_Thread) {
        Stop();
        m_Thread->join(); // TODO
        delete m_Thread;
        m_Thread = nullptr;
    }
}

void DecoderBase::Start() {
    LOGCATI("DecoderBase::Start()");
    if(m_Thread == nullptr) {
        StartDecodingThread();
    } else {

    }
}

int DecoderBase::InitFFDecoder() {
    int result = -1;
    do {
        // 1.创建封装格式上下文
        m_AVFormatContext = avformat_alloc_context();

        // 2.打开文件
        if (avformat_open_input(&m_AVFormatContext, m_Url, NULL, NULL) != 0) {
            LOGCATE("DecoderBase::InitFFDecoder avformat_open_input fail.");
            break;
        }
        // 3.获取音视频流信息
        if (avformat_find_stream_info(m_AVFormatContext, NULL) < 0) {
            LOGCATE("DecoderBase::InitFFDecoder avformat_find_stream_info fail.");
            break;
        }
        // 4.获取音视频流索引
        for (int i = 0; i < m_AVFormatContext->nb_streams; i ++) {
            int codecType = m_AVFormatContext->streams[i]->codecpar->codec_type;
            LOGCATI("DecoderBase::InitFFDecoder stream codec_type=%d", codecType);
            if (codecType == m_MediaType) {
                m_StreamIndex = i;
                break;
            }
        }
        if (m_StreamIndex == -1) {
            LOGCATE("DecoderBase::InitFFDecoder fail to find stream index.");
            break;
        }
        // 5.获取解码器参数
        AVCodecParameters *codecParameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;
        // 6.获取解码器
        m_AVCodec = avcodec_find_decoder(codecParameters->codec_id);
        if (m_AVCodec == nullptr) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_find_decoder fail.");
            break;
        }
        // 7.创建解码器上下文
        m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
        if (avcodec_parameters_to_context(m_AVCodecContext, codecParameters) != 0) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_parameters_to_context fail.");
            break;
        }

        AVDictionary *pAVDictionary = nullptr;
        av_dict_set(&pAVDictionary, "buffer_size", "1024000", 0);
        av_dict_set(&pAVDictionary, "stimeout", "20000000", 0);
        av_dict_set(&pAVDictionary, "max_delay", "30000000", 0);
        av_dict_set(&pAVDictionary, "rtsp_transport", "tcp", 0);

        result = avcodec_open2(m_AVCodecContext, m_AVCodec, &pAVDictionary);
        if (result < 0) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_open2 fail result=%d", result);
            break;
        }

        m_Duration = m_AVFormatContext->duration / AV_TIME_BASE * 1000;
        m_Packet = av_packet_alloc();
        m_Frame = av_frame_alloc();

        LOGCATI("DecoderBase::InitFFDecoder init success!!!!!!! m_MediaType=%d duration=%ld", m_MediaType, m_Duration);
        result = 0;
    } while (false);

    if (result != 0 && m_MsgContext && m_MessageCallback) {
        m_MessageCallback(m_MsgContext, MSG_DECODER_INIT_ERROR, 0);
    }

    return result;
}

void DecoderBase::DecodingLoop() {
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_DecoderState = STATE_DECODING;
        lock.unlock();
    }
    for(;;) {
        if (m_DecoderState == STATE_STOP) {
            break;
        }
        if (m_StartTimeStamp == -1) {
            m_StartTimeStamp = GetSysCurrentTime();
        }
        if (DecodeOnePacket() != 0) {

            m_DecoderState = STATE_STOP; // 解码结束，先这样写，后边再改
        }
    }
    LOGCATE("DecoderBase::DecodingLoop end");
}

int DecoderBase::DecodeOnePacket() {
    int result = av_read_frame(m_AVFormatContext, m_Packet);
    while (result == 0) {
        if (m_Packet->stream_index == m_StreamIndex) {
            if (avcodec_send_packet(m_AVCodecContext, m_Packet) == AVERROR_EOF) {
                // 解码结束
                result = -1;
                break;
            }
            int frameCount = 0;
            while(avcodec_receive_frame(m_AVCodecContext, m_Frame) == 0) {
                // 更新时间戳
                UpdateTimeStamp();
                // 同步
                Async();
                // 渲染
                OnFrameAvailable(m_Frame);
//                LOGCATI("DecoderBase::DecodeOnePacket avcodec_receive_frame");
                frameCount ++;
            }
            LOGCATI("DecoderBase::DecodeOnePacket frameCount=%d", frameCount);
            if (frameCount > 0) {
                // 一个Packet解码完成
                result = 0;
                break;
            }
        }
        av_packet_unref(m_Packet);
        result = av_read_frame(m_AVFormatContext, m_Packet);
    }
//    LOGCATI("DecoderBase::DecodeOnePacket return -=-=-=-=-= %d", result);
    return result;
}


void DecoderBase::UpdateTimeStamp() {
//    LOGCATD("DecoderBase::UpdateTimeStamp == %ld  ---  %ld", m_Frame->pkt_dts, m_Frame->pts);
    if (m_Frame->pts != AV_NOPTS_VALUE) {
        m_CurTimeStamp = m_Frame->pts;
    } else if (m_Frame->pkt_dts != AV_NOPTS_VALUE){
        m_CurTimeStamp = m_Frame->pkt_dts;
    } else {
        m_CurTimeStamp = 0;
    }

    m_CurTimeStamp = (int64_t)((double )m_CurTimeStamp * av_q2d(m_AVFormatContext->streams[m_StreamIndex]->time_base) * 1000);
    LOGCATD("DecoderBase::UpdateTimeStamp m_MediaType=%d time= %ld", m_MediaType, m_CurTimeStamp);

}

long DecoderBase::Async() {
    if (m_MediaType == AVMEDIA_TYPE_AUDIO) {
        AsyncUtil::getInstance().SetAudioPts(m_CurTimeStamp);
        if (m_MsgContext && m_MessageCallback) {
            m_MessageCallback(m_MsgContext, MSG_DECODING_TIME, (float) m_CurTimeStamp / 1000.0f);
        }
//        long delay = m_CurTimeStamp - m_LastedTime;
//        if (delay > 0) {
//            LOGCATD("DecoderBase::Async Audio delay=======%ld", delay);
//            av_usleep(delay / 2 * 1000);
//        }
//        m_LastedTime = m_CurTimeStamp;
        return 0;
    } else {
        long delay = AsyncUtil::getInstance().CalculateDiff(m_CurTimeStamp);
        delay = delay > DELAY_THRESHOLD ? DELAY_THRESHOLD : delay;
        if (delay > 0) {
            LOGCATD("DecoderBase::Async Video delay=======%ld", delay);
            av_usleep(delay * 1000);
        }
//        long elapsedTime = GetSysCurrentTime() - m_StartTimeStamp;
//        if (m_CurTimeStamp > elapsedTime) {
//            LOGCATD("DecoderBase::Async m_MediaType=%d m_CurTimeStamp=%ld, elapsedTime=%ld", m_MediaType, m_CurTimeStamp, elapsedTime);
//            auto sleepTime = static_cast<unsigned int>(m_CurTimeStamp - elapsedTime);
//            sleepTime = sleepTime > DELAY_THRESHOLD ? DELAY_THRESHOLD : sleepTime;
//            av_usleep(sleepTime * 1000);
//        }
//        delay = elapsedTime - m_CurTimeStamp;
        return delay;
    }
}

void DecoderBase::StartDecodingThread() {
    m_Thread = new thread(DoAVDecoding, this);
}

void DecoderBase::DoAVDecoding(DecoderBase * decoder) {
    do {
       if (decoder->InitFFDecoder() != 0) {
           break;
       }
       decoder->OnDecoderReady();
       decoder->DecodingLoop();
    } while (false);
    decoder->UnInitFFDecoder();
    decoder->OnDecoderDone();
}

void DecoderBase::Stop() {

}

void DecoderBase::UnInitFFDecoder() {
    LOGCATD("DecoderBase::UnInitFFDecoder");
    if(m_Frame != nullptr) {
        av_frame_free(&m_Frame);
        m_Frame = nullptr;
    }
    if(m_Packet != nullptr) {
        av_packet_free(&m_Packet);
        m_Packet = nullptr;
    }
    if(m_AVCodecContext != nullptr) {
        avcodec_close(m_AVCodecContext);
        avcodec_free_context(&m_AVCodecContext);
        m_AVCodecContext = nullptr;
        m_AVCodec = nullptr;
    }
    if(m_AVFormatContext != nullptr) {
        avformat_close_input(&m_AVFormatContext);
        avformat_free_context(m_AVFormatContext);
        m_AVFormatContext = nullptr;
    }
}
