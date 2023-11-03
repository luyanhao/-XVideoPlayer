//
// Created by lyhao on 2023/10/23.
//

#ifndef XVIDEOPLAYER_DECODERBASE_H
#define XVIDEOPLAYER_DECODERBASE_H

#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavutil/avutil.h>
#include "libavformat/avformat.h"
}

#include <thread>
#include "Decoder.h"

#define MAX_PATH  2048

using namespace std;

class DecoderBase : public Decoder {
public:
    DecoderBase(){};
    virtual~ DecoderBase(){};
    virtual void Start();
    virtual void Stop();

protected:
    virtual int Init(const char* url, AVMediaType mediaType);
    virtual void UnInit();

private:
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    char m_Url[MAX_PATH] = {0};

    thread *m_Thread = nullptr;

    void StartDecodingThread();
    static void DoAVDecoding(DecoderBase *decoder);
    int InitFFDecoder();

    long m_Duration = 0; //ms
    AVPacket *m_Packet = nullptr;
    AVFrame * m_Frame = nullptr;
    AVFormatContext *m_AVFormatContext = nullptr;
    AVCodec *m_AVCodec = nullptr;
    AVCodecContext *m_AVCodecContext = nullptr;
    int m_StreamIndex = -1;

};


#endif //XVIDEOPLAYER_DECODERBASE_H
