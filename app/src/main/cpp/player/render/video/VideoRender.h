//
// Created by lyhao on 2023/11/10.
//

#ifndef XVIDEOPLAYER_VIDEORENDER_H
#define XVIDEOPLAYER_VIDEORENDER_H

#include "ImageDef.h"

#define VIDEO_RENDER_ANWINDOW  1

class VideoRender {
public:
    VideoRender(int type) {
        m_RenderType = type;
    }
    virtual ~VideoRender(){};
    virtual void Init(int videoWidth, int videoHeight, int *dstSize) = 0;
    virtual void RenderVideoFrame(NativeImage *pImage) = 0;
    virtual void UnInit() = 0;

    int GetRenderType() const {
        return m_RenderType;
    }
private:
    int m_RenderType = VIDEO_RENDER_ANWINDOW;
};

#endif //XVIDEOPLAYER_VIDEORENDER_H
