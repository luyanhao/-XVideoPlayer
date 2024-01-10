//
// Created by lyhao on 2024/1/4.
//

#ifndef XVIDEOPLAYER_VIDEOGLRENDER_H
#define XVIDEOPLAYER_VIDEOGLRENDER_H


#include <__mutex_base>
#include <GLES3/gl3.h>
#include "BaseGLRender.h"
#include "VideoRender.h"
#include "vec2.hpp"

using namespace glm;
#define TEXTURE_NUM 3

class VideoGLRender : public VideoRender, public BaseGLRender{
public:
    virtual void Init(int videoWidth, int videoHeight, int *dstSize);
    virtual void RenderVideoFrame(NativeImage *pImage);
    virtual void UnInit();

    virtual void OnSurfaceCreated();
    virtual void OnSurfaceChanged(int w, int h);
    virtual void OnDrawFrame();

    static VideoGLRender *GetInstance();
    static void ReleaseInstance();

    virtual void UpdateMVPMatrix(int angleX, int angleY, float scaleX, float scaleY);
    virtual void UpdateMVPMatrix(TransformMatrix * pTransformMatrix);
    virtual void SetTouchLoc(float touchX, float touchY) {
//        m_TouchXY.x = touchX / m_ScreenSize.x;
//        m_TouchXY.y = touchY / m_ScreenSize.y;
    }

private:
    VideoGLRender();
    virtual ~VideoGLRender();
    static std::mutex m_Mutex;
    static VideoGLRender *s_Instance;

    GLuint m_ProgramObj = GL_NONE;
    GLuint m_TextureIds[TEXTURE_NUM];
    GLuint m_VaoId;
    GLuint m_VboIds[3];

    vec2 m_TouchXY;
};


#endif //XVIDEOPLAYER_VIDEOGLRENDER_H
