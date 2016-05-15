#pragma once

// Includes
#include <GL/glew.h>


// Includes GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "core/core.h"

// Classe

class SceneOpenGL
{
    public:

    SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre);
    ~SceneOpenGL();

    bool initialiserFenetre();
    bool initGL();
    void bouclePrincipale();


    private:
    void displayPass(glm::vec3 &camPos, Node& mainNode, const glm::mat4& view, const glm::mat4& projection);
    void shadowMapPass(Light &light, Node& mainNode, const glm::mat4& projection);

    std::string m_titreFenetre;
    int m_largeurFenetre;
    int m_hauteurFenetre;

    SDL_Window* m_fenetre;
    SDL_GLContext m_contexteOpenGL;
    SDL_Event m_evenements;

	Shader shader;
	FrameBufferObject fbo;
};
