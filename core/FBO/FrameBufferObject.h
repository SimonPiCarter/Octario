#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

struct CameraDirection
{
    static const size_t NUM_OF_LAYERS = 6;
    GLenum CubemapFace;
    glm::vec3 Target;
    glm::vec3 Up;
};
const CameraDirection cameraDirection[CameraDirection::NUM_OF_LAYERS] =
{
    { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, -1.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
};

class FrameBufferObject
{
    public:

        FrameBufferObject();
        virtual ~FrameBufferObject();

        bool init(unsigned int WindowWidth, unsigned int WindowHeight);

        void bindForWriting(GLenum CubeFace);

        void bindForReading(GLenum TextureUnit);

    private:
        GLuint m_fbo;
        GLuint m_shadowMap;
        GLuint m_depth;
};

