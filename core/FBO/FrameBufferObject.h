#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

class Node;
class Light;
class ShadowMapShader;
class Shader;

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
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
};

class FrameBufferObject
{
    public:

        FrameBufferObject(size_t size_p, size_t textureSize_p);
        virtual ~FrameBufferObject();

        bool init();

        bool init(size_t textureId_p);

        void bindForWriting(size_t textureId_p, GLenum CubeFace);

        void bindForReading(size_t textureId_p, GLenum TextureUnit);

        void shadowPass(size_t textureId_p, Light &light, Node& mainNode, const glm::mat4& projection);

        void debugMode(Light &light, Node& mainNode, const glm::mat4& projection, int x, int y, int width, int height, Shader& shader);

    private:
        const size_t m_nbTexture;
        const size_t m_textureSize;
        GLuint m_fbo;
        GLuint m_depth;
        GLuint* m_shadowMap;

        ShadowMapShader* m_shadowMapShader;
};

