#include "SceneOpenGL.h"

#include <iostream>
#include <sstream>

// Permet d'�viter la r�-�criture du namespace glm::
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


    // Cr�ation de la fen�tre
    m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(m_fenetre == 0)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Cr�ation du contexte OpenGL

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


	// Si l'initialisation a �chou� :
	if(initialisationGLEW != GLEW_OK)
	{
		// On affiche l'erreur gr�ce � la fonction : glewGetErrorString(GLenum code)
		std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;
		// On quitte la SDL
		SDL_GL_DeleteContext(m_contexteOpenGL);
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();

		return false;
	}

    // Activation du Depth Buffer


    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    return true;
}

void SceneOpenGL::bouclePrincipale()
{
    // Variables

    bool terminer(false);
    float taille = 1.f;
    // Division du param�tre taille
	taille /= 2.f;

	shader.load();

	DrawableModel model = DrawableFactory::get().createCubeSampleTextureModel(2.0f);
	Drawable cube(&model,&shader);
	cube.load();

	DrawableModel modelPlane = DrawableFactory::get().createPlaneModel(7.f,4.f,0.1f,0.5f,0.5f,0.5f);
	Drawable plane(&modelPlane,&shader);
	plane.load();

	Node mainNode;
	mainNode.addDrawable("cube",&cube);
	mainNode.addDrawable("plane",&plane);

	Node subNode;
	subNode.addDrawable("cube",&cube);
	subNode.translate(2,2,0);
	mainNode.addSubNode("subNode",&subNode);

	// Load light
	Light light;
	light.translate(5,5,5);
	light.setProperties(1,1,1,50);
	Light light2;
	light2.translate(-7,-3,0);
	light2.setProperties(1,0.2,0.2,50);

	LightManager::get().addPointLight("testLight",&light);
	LightManager::get().addPointLight("testLight2",&light2);

    // Matrices
    mat4 projection;
    mat4 modelview;

    projection = perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
    modelview = lookAt(vec3(5, 5, 5), vec3(0, 0, 0), vec3(0, 1, 0));


	Uint32 start_time = SDL_GetTicks();
	Uint32 elapsed_time = 0;
	Uint32 frame_count = 0;

	SDL_GL_SetSwapInterval(0);

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

        standardDisplay(mainNode,modelview,projection);

        // display swap buffer
        SDL_GL_SwapWindow(m_fenetre);
    }
}

void SceneOpenGL::standardDisplay(Node& mainNode, const mat4& modelview, const mat4& projection) {
    glUseProgram(shader.getProgramID());

    glUniform4fv(glGetUniformLocation(shader.getProgramID(), "pointLightPos"),
                LightManager::get().getPointLightCount(),
                LightManager::get().getPointLightsPos());
    glUniform4fv(glGetUniformLocation(shader.getProgramID(), "pointLightProp"),
                LightManager::get().getPointLightCount(),
                LightManager::get().getPointLightsProp());
    glUniform1i(glGetUniformLocation(shader.getProgramID(), "pointLightCount"),
                LightManager::get().getPointLightCount());

    glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "view"), 1, GL_FALSE, value_ptr(modelview));
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

    // Nettoyage de l'ecran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotation du repere
    mainNode.rotate(vec3(0, 1, 0),0.05f);

    mainNode.draw(modelview,glm::mat4(1), projection,shader);
}
