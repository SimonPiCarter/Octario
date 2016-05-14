#include "FrameBufferObject.h"

#include "../Light/Light.h"
#include "../Light/LightManager.h"
#include "../Node/Node.h"
#include "../Shader/ShadowMapShader.h"
#include "../Shader/Shader.h"

#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

FrameBufferObject::FrameBufferObject() : m_fbo(0),m_shadowMap(0), m_shadowMapShader(new ShadowMapShader())
{
}

FrameBufferObject::~FrameBufferObject()
{
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
    }

    if (m_shadowMap != 0) {
        glDeleteTextures(1, &m_shadowMap);
    }

    delete m_shadowMapShader;
}

bool FrameBufferObject::init(unsigned int textureSize)
{
	m_shadowMapShader->load();

    m_textureSize = textureSize;
    // Create the FBO
    glGenFramebuffers(1, &m_fbo);

	// Create the cube map
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

    for (glm::uint i = 0 ; i < 6 ; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, textureSize, textureSize, 0, GL_RED, GL_FLOAT, NULL);
    }

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout<<"FB error, status: "<<Status<<std::endl;
        return false;
    }

    int glError = glGetError();
    if ( glError != GL_NO_ERROR ) {
        std::cout<<"GL error, status: "<<glError<<std::endl;
        return false;
    }

    return true ;
}

void FrameBufferObject::shadowPass(Light &light, Node& mainNode, const glm::mat4& projection) {
    glCullFace(GL_FRONT);

    glm::mat4 view;
    glm::vec4 lightPos4 = light.getPosition();
    glm::vec3 lightPos3(lightPos4.x,lightPos4.y,lightPos4.z);

    glUseProgram(m_shadowMapShader->getProgramID());

    glUniform3f(glGetUniformLocation(m_shadowMapShader->getProgramID(), "lightPos"), lightPos4.x,lightPos4.y,lightPos4.z);

    glViewport(0,0,m_textureSize,m_textureSize);

    // Clear screen
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

    for (size_t i = 0 ; i < CameraDirection::NUM_OF_LAYERS ; ++i) {
        bindForWriting(cameraDirection[i].CubemapFace);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        view = glm::lookAt(lightPos3, lightPos3+cameraDirection[i].Target, cameraDirection[i].Up);

        mainNode.draw(view, glm::mat4(1), projection, *m_shadowMapShader);
    }
}

void FrameBufferObject::bindForWriting(GLenum CubeFace)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CubeFace, m_shadowMap, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}


void FrameBufferObject::bindForReading(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
}

void FrameBufferObject::debugMode(Light &light, Node& mainNode, const glm::mat4& projection, int x, int y, int width, int height, Shader& shader) {
    glCullFace(GL_FRONT);

    glm::mat4 view;
    glm::vec4 lightPos4 = light.getPosition();
    glm::vec3 lightPos3(lightPos4.x,lightPos4.y,lightPos4.z);

    glUseProgram(shader.getProgramID());

    glUniform4fv(glGetUniformLocation(shader.getProgramID(), "pointLightPos"),
                LightManager::get().getPointLightCount(),
                LightManager::get().getPointLightsPos());
    glUniform4fv(glGetUniformLocation(shader.getProgramID(), "pointLightProp"),
                LightManager::get().getPointLightCount(),
                LightManager::get().getPointLightsProp());
    glUniform1i(glGetUniformLocation(shader.getProgramID(), "pointLightCount"),
                LightManager::get().getPointLightCount());

    glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Clear screen
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

    for (size_t i = 0 ; i < CameraDirection::NUM_OF_LAYERS ; ++i) {
        glViewport(x+width*(i%3),y+height/3,width,height);
        //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        view = glm::lookAt(lightPos3, lightPos3+cameraDirection[i].Target, cameraDirection[i].Up);

        glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        mainNode.draw(view, glm::mat4(1), projection, shader);
    }
}
