//
// Created by lyhao on 2024/1/4.
//

#include "VideoGLRender.h"
#include "LogUtil.h"
#include "GLUtils.h"

VideoGLRender *VideoGLRender::s_Instance = nullptr;
std::mutex VideoGLRender::m_Mutex;

static char vShaderStr[] =
        "#version 300 es\n"
        "layout(location = 0) in vec4 a_position;\n"
        "layout(location = 1) in vec2 a_texCoord;\n"
        "uniform mat4 u_MVPMatrix;\n"
        "out vec2 v_texCoord;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = u_MVPMatrix * a_position;\n"
        "    v_texCoord = a_texCoord;\n"
        "}";

static char fShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform sampler2D s_texture0;\n"
        "uniform sampler2D s_texture1;\n"
        "uniform sampler2D s_texture2;\n"
        "uniform int u_nImgType;// 1:RGBA, 2:NV21, 3:NV12, 4:I420\n"
        "\n"
        "void main()\n"
        "{\n"
        "\n"
        "    if(u_nImgType == 1) //RGBA\n"
        "    {\n"
        "        outColor = texture(s_texture0, v_texCoord);\n"
        "    }\n"
        "    else if(u_nImgType == 2) //NV21\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, v_texCoord).r;\n"
        "        yuv.y = texture(s_texture1, v_texCoord).a - 0.5;\n"
        "        yuv.z = texture(s_texture1, v_texCoord).r - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "        0.0, \t-0.344, \t1.770,\n"
        "        1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "\n"
        "    }\n"
        "    else if(u_nImgType == 3) //NV12\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, v_texCoord).r;\n"
        "        yuv.y = texture(s_texture1, v_texCoord).r - 0.5;\n"
        "        yuv.z = texture(s_texture1, v_texCoord).a - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "        0.0, \t-0.344, \t1.770,\n"
        "        1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "    }\n"
        "    else if(u_nImgType == 4) //I420\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, v_texCoord).r;\n"
        "        yuv.y = texture(s_texture1, v_texCoord).r - 0.5;\n"
        "        yuv.z = texture(s_texture2, v_texCoord).r - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "                              0.0, \t-0.344, \t1.770,\n"
        "                              1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        outColor = vec4(1.0);\n"
        "    }\n"
        "}";

static char fMeshShaderStr[] =
        "//dynimic mesh 动态网格\n"
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform sampler2D s_texture0;\n"
        "uniform sampler2D s_texture1;\n"
        "uniform sampler2D s_texture2;\n"
        "uniform int u_nImgType;// 1:RGBA, 2:NV21, 3:NV12, 4:I420\n"
        "uniform float u_Offset;\n"
        "uniform vec2 u_TexSize;\n"
        "\n"
        "vec4 sampleImage(vec2 texCoord) {\n"
        "    vec4 outColor;\n"
        "    if(u_nImgType == 1) //RGBA\n"
        "    {\n"
        "        outColor = texture(s_texture0, texCoord);\n"
        "    }\n"
        "    else if(u_nImgType == 2) //NV21\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, texCoord).r;\n"
        "        yuv.y = texture(s_texture1, texCoord).a - 0.5;\n"
        "        yuv.z = texture(s_texture1, texCoord).r - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "        0.0,  -0.344,  1.770,\n"
        "        1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "\n"
        "    }\n"
        "    else if(u_nImgType == 3) //NV12\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, texCoord).r;\n"
        "        yuv.y = texture(s_texture1, texCoord).r - 0.5;\n"
        "        yuv.z = texture(s_texture1, texCoord).a - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "        0.0,  -0.344,  1.770,\n"
        "        1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "    }\n"
        "    else if(u_nImgType == 4) //I420\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, texCoord).r;\n"
        "        yuv.y = texture(s_texture1, texCoord).r - 0.5;\n"
        "        yuv.z = texture(s_texture2, texCoord).r - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "        0.0,  -0.344,  1.770,\n"
        "        1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        outColor = vec4(1.0);\n"
        "    }\n"
        "    return outColor;\n"
        "}\n"
        "void main()\n"
        "{\n"
        "    vec2 imgTexCoord = v_texCoord * u_TexSize;\n"
        "    float sideLength = u_TexSize.y / 6.0;\n"
        "    float maxOffset = 0.08 * sideLength;\n"
        "    float x = mod(imgTexCoord.x, floor(sideLength));\n"
        "    float y = mod(imgTexCoord.y, floor(sideLength));\n"
        "\n"
        "    float offset = u_Offset * maxOffset;\n"
        "\n"
        "    if(offset <= x\n"
        "    && x <= sideLength - offset\n"
        "    && offset <= y\n"
        "    && y <= sideLength - offset)\n"
        "    {\n"
        "        outColor = sampleImage(v_texCoord);\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        outColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "    }\n"
        "}";

static char fGrayShaderStr[] =
        "//黑白滤镜\n"
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform sampler2D s_TextureMap;//采样器\n"
        "void main()\n"
        "{\n"
        "    outColor = texture(s_TextureMap, v_texCoord);\n"
        "    if(v_texCoord.x > 0.5)\n"
        "        outColor = vec4(vec3(outColor.r*0.299 + outColor.g*0.587 + outColor.b*0.114), outColor.a);\n"
        "}";

GLfloat verticesCoords[] = {
        -1.0f,  1.0f, 0.0f,  // Position 0
        -1.0f, -1.0f, 0.0f,  // Position 1
        1.0f,  -1.0f, 0.0f,  // Position 2
        1.0f,   1.0f, 0.0f,  // Position 3
};

GLfloat textureCoords[] = {
        0.0f,  0.0f,        // TexCoord 0
        0.0f,  1.0f,        // TexCoord 1
        1.0f,  1.0f,        // TexCoord 2
        1.0f,  0.0f         // TexCoord 3
};
GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
void VideoGLRender::Init(int videoWidth, int videoHeight, int *dstSize) {

}

void VideoGLRender::RenderVideoFrame(NativeImage *pImage) {

}

void VideoGLRender::UnInit() {

}

void VideoGLRender::OnSurfaceCreated() {
    LOGCATE("VideoGLRender::OnSurfaceCreated");

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);
    if (!m_ProgramObj)
    {
        LOGCATE("VideoGLRender::OnSurfaceCreated create program fail");
        return;
    }

    glGenTextures(TEXTURE_NUM, m_TextureIds);
    for (int i = 0; i < TEXTURE_NUM ; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_TextureIds[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }

    // Generate VBO Ids and load the VBOs with data
    glGenBuffers(3, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Generate VAO Id
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);

    glBindVertexArray(GL_NONE);

    m_TouchXY = vec2(0.5f, 0.5f);
}

void VideoGLRender::OnSurfaceChanged(int w, int h) {

}

void VideoGLRender::OnDrawFrame() {

}

VideoGLRender *VideoGLRender::GetInstance() {
    if (s_Instance == nullptr) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (s_Instance == nullptr) {
            s_Instance = new VideoGLRender();
        }
    }
    return s_Instance;
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
