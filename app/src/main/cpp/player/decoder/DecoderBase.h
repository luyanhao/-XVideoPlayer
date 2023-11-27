//
// Created by lyhao on 2023/10/23.
//

#ifndef XVIDEOPLAYER_DECODERBASE_H
#define XVIDEOPLAYER_DECODERBASE_H

#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavutil/avutil.h>
#include "libavformat/avformat.h"
#include <libavutil/time.h>
}

#include <thread>
#include "Decoder.h"

#define MAX_PATH  2048
#define DELAY_THRESHOLD 100 //100ms

using namespace std;

enum DecoderState {
    STATE_UNKNOWN,
    STATE_DECODING,
    STATE_PAUSE,
    STATE_STOP
};

enum DecoderMsg {
    MSG_DECODER_INIT_ERROR,
    MSG_DECODER_READY,
    MSG_DECODER_DONE,
    MSG_REQUEST_RENDER,
    MSG_DECODING_TIME
};

class DecoderBase : public Decoder {
public:
    DecoderBase(){};
    virtual~ DecoderBase(){};
    virtual void Start();
    virtual void Stop();

    virtual void SetMessageCallback(void *context, MessageCallback callback) {
        m_MsgContext = context;
        m_MessageCallback = callback;
    }

protected:
    virtual int Init(const char* url, AVMediaType mediaType);
    virtual void UnInit();

    void *m_MsgContext = nullptr;
    MessageCallback m_MessageCallback = nullptr;

    virtual void OnDecoderReady() = 0;
    virtual void OnFrameAvailable(AVFrame *avFrame) = 0;
    virtual void OnDecoderDone() = 0;
    AVCodecContext *GetCodeContext() {
        return m_AVCodecContext;
    }
private:
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    char m_Url[MAX_PATH] = {0};

    thread *m_Thread = nullptr;
    mutex m_Mutex;
    DecoderState m_DecoderState = STATE_UNKNOWN;

    void DecodingLoop();
    int DecodeOnePacket();
    void UpdateTimeStamp();
    long Async();

    void StartDecodingThread();
    static void DoAVDecoding(DecoderBase *decoder);
    int InitFFDecoder();
    void UnInitFFDecoder();

    long m_Duration = 0; //ms
    AVPacket *m_Packet = nullptr;
    AVFrame * m_Frame = nullptr;
    AVFormatContext *m_AVFormatContext = nullptr;
    AVCodec *m_AVCodec = nullptr;
    AVCodecContext *m_AVCodecContext = nullptr;
    int m_StreamIndex = -1;

    long m_CurTimeStamp = 0;
    long m_StartTimeStamp = -1;
};


#endif //XVIDEOPLAYER_DECODERBASE_H
