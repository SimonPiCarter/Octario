#include "SceneOpenGL.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace glm;

SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) : m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre),
                                                                                             m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0),
                                                                                             fbo(2,1024)
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

    SDL_SetRelativeMouseMode(SDL_TRUE);


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

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);


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
    glEnable(GL_CULL_FACE);

    return true;
}

void SceneOpenGL::bouclePrincipale()
{
    // Variables

    bool over(false);
    float taille = 1.f;
    // Division du param�tre taille
	taille /= 2.f;

	shader.load();
	debugShader.load();

	//DrawableModel model = DrawableFactory::get().createCubeSampleTextureModel(2.0f);
	DrawableModel* model = DrawableFactory::get().loadModelFromFile("Textures/demo","column.obj");
	Drawable cube(model);
	cube.load();

	DrawableModel* modelRoom = DrawableFactory::get().loadModelFromFile("Textures/demo","room.obj");
	Drawable room(modelRoom);
	room.load();

	DrawableModel modelPlane = DrawableFactory::get().createPlaneModel(7.f,4.f,1.f,0.5f,0.5f,0.5f,"Textures/cobblestone.jpg","Textures/cobblestoneNormal.jpg","Textures/cobblestoneBump.jpg");
	Drawable plane(&modelPlane);
	plane.load();

	Node mainNode;

	Node roomNode;
	roomNode.addDrawable("room",&room);
	mainNode.addSubNode("roomNode",&roomNode);

	/*Node subNode1;
	subNode1.addDrawable("cube",&cube);
	subNode1.addDrawable("plane",&plane);
	roomNode.addSubNode("subNode1",&subNode1);

*/
    for ( int i = 0 ; i < 15 ; ++ i ) {
        std::stringstream convert; // stringstream used for the conversion
        convert << i;//add the value of Number to the characters in the stream
        std::string sti = convert.str();//set Result to the content of the stream

        Node* subNode = new Node();
        subNode->addDrawable("cube",&cube);
        subNode->translate(-3.5,2.9,3.5-i*1.2);
        roomNode.addSubNode("subNode1"+sti,subNode);

        Node* subNode2 = new Node();
        subNode2->addDrawable("cube",&cube);
        subNode2->translate(3.0,2.9,3.5-i*1.2);
        roomNode.addSubNode("subNode2"+sti,subNode2);
    }

	// Load columns


	// Load light
	Light light;
	light.translate(3.5,5,0);
	light.setProperties(1,1,1,20);
	Light light2;
	light2.translate(-3,5,0);
	light2.setProperties(1,0.2,0.2,20);

	LightManager::get().addPointLight("testLight",&light);
	LightManager::get().addPointLight("testLight2",&light2);

    // Matrices
    mat4 projection;
    mat4 shadowMapProjection;
    mat4 view;
    Camera camera;
    camera.translate(0,4,4);

    projection = perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, 0.1, 100.0);
    shadowMapProjection = perspective(90.0, 1.0, 0.1, 100.0);

    fbo.init();
    fbo.init(0);
    fbo.init(1);

	Uint32 start_time = SDL_GetTicks();
	Uint32 elapsed_time = 0;
	Uint32 frame_count = 0;

	SDL_GL_SetSwapInterval(0);

	camera.yaw(180.f);

    // Boucle principale
    while(!over)
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
        while (SDL_PollEvent(&m_evenements)){
            //If user closes the window
            if (m_evenements.type == SDL_QUIT){
                over = true;
            }
            else if(m_evenements.window.event == SDL_WINDOWEVENT_CLOSE) {
                over = true;
            }
            else if (m_evenements.type == SDL_KEYDOWN ) {
                float speed = 0.1f;
                if ( m_evenements.key.keysym.sym == SDLK_UP || m_evenements.key.keysym.sym == SDLK_z ) {
                    camera.translate(0,0,speed);
                } else if ( m_evenements.key.keysym.sym == SDLK_DOWN || m_evenements.key.keysym.sym == SDLK_s ) {
                    camera.translate(0,0,-speed);
                } else if ( m_evenements.key.keysym.sym == SDLK_RIGHT || m_evenements.key.keysym.sym == SDLK_d ) {
                    camera.translate(speed,0,0);
                } else if ( m_evenements.key.keysym.sym == SDLK_LEFT || m_evenements.key.keysym.sym == SDLK_q ) {
                    camera.translate(-speed,0,0);
                } else if ( m_evenements.key.keysym.sym == SDLK_KP_0 || m_evenements.key.keysym.sym == SDLK_r ) {
                    roomNode.rotate(vec3(0, 1, 0),0.5f);
                } else if ( m_evenements.key.keysym.sym == SDLK_KP_1 || m_evenements.key.keysym.sym == SDLK_e ) {
                    roomNode.rotate(vec3(0, 1, 0),-0.5f);
                } else if ( m_evenements.key.keysym.sym == SDLK_ESCAPE ) {
                    over = true;
                }
            }
            else if ( m_evenements.type == SDL_MOUSEBUTTONDOWN ) {

            }
            else if ( m_evenements.type == SDL_MOUSEMOTION ) {
                float angleYaw = -(float)m_evenements.motion.xrel/(float)m_largeurFenetre*70.f;
                float anglePitch = -(float)m_evenements.motion.yrel/(float)m_hauteurFenetre*70.f;
                camera.pitch(anglePitch);
                camera.yaw(angleYaw);
            }
        }

        view = camera.getView();

        fbo.shadowPass(0,light,mainNode,shadowMapProjection);
        fbo.shadowPass(1,light2,mainNode,shadowMapProjection);

        displayPass(camera.getPosition(),mainNode,view,projection);
        //debugPass(mainNode,view,projection);

        //fbo.debugMode(light2,mainNode,shadowMapProjection,0,0,256,256,shader);

        // display swap buffer
        SDL_GL_SwapWindow(m_fenetre);
    }
}

void SceneOpenGL::displayPass(vec3 camPos, Node& mainNode, const mat4& view, const mat4& projection) {
    glCullFace(GL_BACK);

    glViewport(0,0,m_largeurFenetre,m_hauteurFenetre);

    // Clear screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    glUniform3fv(glGetUniformLocation(shader.getProgramID(), "camPos_world"), 1, value_ptr(camPos));

    fbo.bindForReading(0,GL_TEXTURE3);
    fbo.bindForReading(1,GL_TEXTURE4);
    for ( unsigned int i = 0 ; i < 14 ; ++ i ) {
        glActiveTexture(GL_TEXTURE5+i);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 3);
    }
    GLint *indexes = new GLint[16]{3,4,5,6,7,3,3,3,3,3,3,3,3,3,3,3};
    glUniform1iv(glGetUniformLocation(shader.getProgramID(), "shadowMap"), 16, indexes);
    glUniform1i(glGetUniformLocation(shader.getProgramID(), "shadowMapCount"), 2);

    mainNode.draw(view,glm::mat4(1), projection,shader);
}

void SceneOpenGL::debugPass(Node& mainNode, const mat4& view, const mat4& projection) {
    glUseProgram(debugShader.getProgramID());

    mainNode.drawDebug(view,glm::mat4(1), projection,debugShader);
}
