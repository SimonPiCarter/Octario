#include "Texture.h"


// Constructeurs et Destructeur

Texture::Texture() : id(0), fileImage("")
{

}


Texture::Texture(Texture const &texture)
{
    fileImage = texture.fileImage;
    load();
}


Texture::Texture(std::string fichierImage) : id(0), fileImage(fichierImage)
{

}


Texture::~Texture()
{
    glDeleteTextures(1, &id);
}


// Méthodes

Texture& Texture::operator=(Texture const &textureACopier)
{
    fileImage = textureACopier.fileImage;
    load();

    return *this;
}


bool Texture::load()
{
    SDL_Surface *imageSDL = IMG_Load(fileImage.c_str());

    if(imageSDL == 0)
    {
        std::cout << "Error : " << SDL_GetError() << std::endl;
        return false;
    }


    SDL_Surface *oppositeImage = inverse(imageSDL);
    SDL_FreeSurface(imageSDL);

    if(glIsTexture(id) == GL_TRUE)
        glDeleteTextures(1, &id);

    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);

    GLenum formatInterne(0);
    GLenum format(0);

    if(oppositeImage->format->BytesPerPixel == 3)
    {
        formatInterne = GL_RGB;

        if(oppositeImage->format->Rmask == 0xff)
            format = GL_RGB;

        else
            format = GL_BGR;
    }

    else if(oppositeImage->format->BytesPerPixel == 4)
    {
        formatInterne = GL_RGBA;

        if(oppositeImage->format->Rmask == 0xff)
            format = GL_RGBA;

        else
            format = GL_BGRA;
    }

    else
    {
        std::cout << "Error, wrong file format" << std::endl;
        SDL_FreeSurface(oppositeImage);

        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, oppositeImage->w, oppositeImage->h, 0, format, GL_UNSIGNED_BYTE, oppositeImage->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(oppositeImage);
    return true;
}


SDL_Surface* Texture::inverse(SDL_Surface *imageSource) const
{
    SDL_Surface *imageInversee = SDL_CreateRGBSurface(0, imageSource->w, imageSource->h, imageSource->format->BitsPerPixel, imageSource->format->Rmask,
                                                         imageSource->format->Gmask, imageSource->format->Bmask, imageSource->format->Amask);
    unsigned char* pixelsSources = (unsigned char*) imageSource->pixels;
    unsigned char* pixelsInverses = (unsigned char*) imageInversee->pixels;

    for(int i = 0; i < imageSource->h; i++)
    {
        for(int j = 0; j < imageSource->w * imageSource->format->BytesPerPixel; j++)
            pixelsInverses[(imageSource->w * imageSource->format->BytesPerPixel * (imageSource->h - 1 - i)) + j] = pixelsSources[(imageSource->w * imageSource->format->BytesPerPixel * i) + j];
    }
    return imageInversee;
}




GLuint Texture::getId() const
{
    return id;
}


void Texture::setFileImage(const std::string &fichierImage)
{
    fileImage = fichierImage;
}

