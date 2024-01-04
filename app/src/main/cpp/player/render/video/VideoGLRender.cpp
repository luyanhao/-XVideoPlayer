//
// Created by lyhao on 2024/1/4.
//

#include "VideoGLRender.h"

void VideoGLRender::Init(int videoWidth, int videoHeight, int *dstSize) {

}

void VideoGLRender::RenderVideoFrame(NativeImage *pImage) {

}

void VideoGLRender::UnInit() {

}

void VideoGLRender::OnSurfaceCreated() {

}

void VideoGLRender::OnSurfaceChanged(int w, int h) {

}

void VideoGLRender::OnDrawFrame() {

}

VideoGLRender *VideoGLRender::GetInstance() {
    return nullptr;
}

void VideoGLRender::ReleaseInstance() {

}

void VideoGLRender::UpdateMVPMatrix(int angleX, int angleY, float scaleX, float scaleY) {

}

void VideoGLRender::UpdateMVPMatrix(TransformMatrix *pTransformMatrix) {

}

VideoGLRender::VideoGLRender(): VideoRender(VIDEO_GL_RENDER) {

}

VideoGLRender::~VideoGLRender() {

}
