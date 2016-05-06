#pragma once

// Include

#ifdef WIN32
#include <GL/glew.h>
#include <SDL2/SDL_image.h>

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <SDL2_image/SDL_image.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include <SDL2/SDL_image.h>

#endif

#include <SDL2/SDL.h>
#include <iostream>
#include <string>


// Classe Textures

class Texture
{
    public:

    Texture();
    Texture(Texture const &textureACopier);
    Texture(std::string fichierImage);
    ~Texture();

    Texture& operator=(Texture const &textureACopier);
    bool load();
    SDL_Surface* inverse(SDL_Surface *imageSource) const;

    GLuint getId() const;
    void setFileImage(const std::string &fichierImage);

    private:

    GLuint id;
    std::string fileImage;
};
