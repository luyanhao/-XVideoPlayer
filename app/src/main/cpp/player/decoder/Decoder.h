
#ifndef XVIDEOPLAYER_DECODER_H
#define XVIDEOPLAYER_DECODER_H

class Decoder {
public:
    virtual void Start() = 0;
    virtual void Stop() = 0;
};

#endif //XVIDEOPLAYER_DECODER_H