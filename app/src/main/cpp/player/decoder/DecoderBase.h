//
// Created by lyhao on 2023/10/23.
//

#ifndef XVIDEOPLAYER_DECODERBASE_H
#define XVIDEOPLAYER_DECODERBASE_H

#define __STDC_CONSTANT_MACROS
extern "C" {
#include <libavutil/avutil.h>
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

};


#endif //XVIDEOPLAYER_DECODERBASE_H
