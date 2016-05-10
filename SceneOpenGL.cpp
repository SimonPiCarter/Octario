#include "SceneOpenGL.h"

#include <iostream>
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
    glCullFace(GL_BACK);

    return true;
}

void SceneOpenGL::bouclePrincipale()
{
    // Variables

    bool terminer(false);
    float taille = 1.f;
    // Division du paramètre taille
	taille /= 2.f;

	shader.load();
	shadowMapShader.load();

	DrawableModel model = DrawableFactory::get().createCubeSampleTextureModel(2.0f);
	Drawable cube(&model);
	cube.load();

	DrawableModel modelPlane = DrawableFactory::get().createPlaneModel(7.f,4.f,0.1f,0.5f,0.5f,0.5f);
	Drawable plane(&modelPlane);
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
    mat4 shadowMapProjection;
    mat4 view;

    projection = perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
    shadowMapProjection = perspective(90.0, (double) m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
    view = lookAt(vec3(5, 5, 5), vec3(0, 0, 0), vec3(0, 1, 0));

    fbo.init(m_largeurFenetre, m_hauteurFenetre);

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

        // Rotation du repere
        mainNode.rotate(vec3(0, 1, 0),0.05f);

        shadowMapPass(light,mainNode,shadowMapProjection);

        displayPass(mainNode,view,projection);

        // display swap buffer
        SDL_GL_SwapWindow(m_fenetre);
    }
}

void SceneOpenGL::displayPass(Node& mainNode, const mat4& view, const mat4& projection) {
    glCullFace(GL_BACK);

    glUseProgram(shader.getProgramID());

    glUniform4fv(glGetUniformLocation(shader.getProgramID(), "pointLightPos"),
                LightManager::get().getPointLightCount(),
                LightManager::get().getPointLightsPos());
    glUniform4fv(glGetUniformLocation(shader.getProgramID(), "pointLightProp"),
                LightManager::get().getPointLightCount(),
                LightManager::get().getPointLightsProp());
    glUniform1i(glGetUniformLocation(shader.getProgramID(), "pointLightCount"),
                LightManager::get().getPointLightCount());

    glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

    fbo.bindForReading(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader.getProgramID(), "shadowMap"), 2);

    // Clear screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainNode.draw(view,glm::mat4(1), projection,shader);
}

void SceneOpenGL::shadowMapPass(Light &light, Node& mainNode, const mat4& projection) {

    mat4 view;
    vec4 lightPos4 = light.getPosition();
    vec3 lightPos3(lightPos4.x,lightPos4.y,lightPos4.z);

    glCullFace(GL_FRONT);

    glUseProgram(shadowMapShader.getProgramID());
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.getProgramID(), "lightPos"), 1, GL_FALSE, value_ptr(lightPos3));

    // Clear screen
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

    for (unsigned int i = 0 ; i < CameraDirection::NUM_OF_LAYERS ; i++) {
        fbo.bindForWriting(cameraDirection[i].CubemapFace);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        view = lookAt(lightPos3, cameraDirection[i].Target, cameraDirection[i].Up);

        glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.getProgramID(), "view"), 1, GL_FALSE, value_ptr(view));

        mainNode.draw(view, glm::mat4(1), projection, shadowMapShader);
    }

}
