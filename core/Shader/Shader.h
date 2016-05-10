#pragma once


// Include Windows

#ifdef WIN32
#include <GL/glew.h>


// Include Mac

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>


// Include UNIX/Linux

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif


// Includes communs

#include <iostream>
#include <string>
#include <fstream>

#include "../Drawable/DrawableModel.h"


// Classe Shader

class Shader
{
    public:

    Shader();
    Shader(Shader const &shaderACopier);
    virtual ~Shader();

    Shader& operator=(Shader const &shaderACopier);

    bool load();
    bool compilerShader(GLuint &shader, GLenum type, std::string const &fichierSource);
    GLuint getProgramID() const;

    virtual void digestModel(DrawableModel* model);

    protected:
    Shader(std::string vertexSource, std::string fragmentSource);

    GLuint m_vertexID;
    GLuint m_fragmentID;
    GLuint m_programID;

    std::string m_vertexSource;
    std::string m_fragmentSource;
};

