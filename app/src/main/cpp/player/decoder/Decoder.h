
#ifndef XVIDEOPLAYER_DECODER_H
#define XVIDEOPLAYER_DECODER_H

typedef void(*MessageCallback)(void*, int, float );

class Decoder {
public:
    virtual void Start() = 0;
    virtual void Stop() = 0;

    virtual void SetMessageCallback(void *context, MessageCallback callback) = 0;
};

#endif //XVIDEOPLAYER_DECODER_H