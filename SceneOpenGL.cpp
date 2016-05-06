#include "SceneOpenGL.h"
#include "core/core.h"

#include <sstream>

// Permet d'éviter la ré-écriture du namespace glm::
using namespace glm;

SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) : m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre),
                                                                                             m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0)
{

}

SceneOpenGL::~SceneOpenGL()
{
    SDL_GL_DeleteContext(m_contexteOpenGL);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
}

bool SceneOpenGL::initialiserFenetre()
{
    // Initialisation de la SDL

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Version d'OpenGL

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    // Double Buffer

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    // Création de la fenêtre
    m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(m_fenetre == 0)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Création du contexte OpenGL

    m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);

    if(m_contexteOpenGL == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_fenetre);
        SDL_Quit();

        return false;
    }

    return true;
}

bool SceneOpenGL::initGL()
{
	// On initialise GLEW
	GLenum initialisationGLEW( glewInit() );


	// Si l'initialisation a échoué :
	if(initialisationGLEW != GLEW_OK)
	{
		// On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)
		std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;
		// On quitte la SDL
		SDL_GL_DeleteContext(m_contexteOpenGL);
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();

		return false;
	}

    // Activation du Depth Buffer


    glEnable(GL_DEPTH_TEST);

    return true;
}

void SceneOpenGL::bouclePrincipale()
{
    // Variables

    bool terminer(false);
    float taille = 1.f;
    // Division du paramètre taille
	taille /= 2.f;

	DrawableModel model = DrawableFactory::get().createCubeSampleTextureModel(2.0f);
	Shader shader("Shaders/texture.vert","Shaders/texture.frag");
	Drawable cube(&model,&shader);
	shader.load();
	cube.load();

	Node mainNode;
	mainNode.addDrawable("cube",&cube);

	Node subNode;
	subNode.addDrawable("cube",&cube);
	subNode.translate(2,2,0);
	mainNode.addSubNode("subNode",&subNode);

    // Matrices
    mat4 projection;
    mat4 modelview;

    projection = perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
    modelview = lookAt(vec3(3, 3, 3), vec3(0, 0, 0), vec3(0, 1, 0));

	Uint32 start_time = SDL_GetTicks();
	Uint32 elapsed_time = 0;
	Uint32 frame_count = 0;

    // Boucle principale
    while(!terminer)
    {
		elapsed_time = SDL_GetTicks()-start_time;
		++frame_count;

		if ( elapsed_time > 1000 ) {
			double frame_rate = (double)frame_count/(double)elapsed_time*1000;
			std::ostringstream strs;
			strs <<"OpenGL FPS: "<<frame_rate;
			std::string str = strs.str();
			SDL_SetWindowTitle(m_fenetre,str.c_str());
			elapsed_time = 0;
			start_time = SDL_GetTicks();
			frame_count = 0;
		}
        // Gestion des evenements
        SDL_PollEvent(&m_evenements);

        if(m_evenements.window.event == SDL_WINDOWEVENT_CLOSE)
            terminer = true;


        // Nettoyage de l'ecran
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rotation du repere
		mainNode.rotate(vec3(0, 1, 0),0.5f);

        mainNode.draw(modelview, projection);

        // Actualisation de la fenêtre
        SDL_GL_SwapWindow(m_fenetre);
    }
}
