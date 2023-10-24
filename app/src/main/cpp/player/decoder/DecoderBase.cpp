//
// Created by lyhao on 2023/10/23.
//

#include "DecoderBase.h"

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