#include "SceneOpenGL.h"
#include "core/core.h"

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


	// Vertices
	float vertices[] = {-taille, -taille, -taille,   taille, -taille, -taille,   taille, taille, -taille,     // Face 1
						   -taille, -taille, -taille,   -taille, taille, -taille,   taille, taille, -taille,     // Face 1

						   taille, -taille, taille,   taille, -taille, -taille,   taille, taille, -taille,       // Face 2
						   taille, -taille, taille,   taille, taille, taille,   taille, taille, -taille,         // Face 2

						   -taille, -taille, taille,   taille, -taille, taille,   taille, -taille, -taille,      // Face 3
						   -taille, -taille, taille,   -taille, -taille, -taille,   taille, -taille, -taille,    // Face 3

						   -taille, -taille, taille,   taille, -taille, taille,   taille, taille, taille,        // Face 4
						   -taille, -taille, taille,   -taille, taille, taille,   taille, taille, taille,        // Face 4

						   -taille, -taille, -taille,   -taille, -taille, taille,   -taille, taille, taille,     // Face 5
						   -taille, -taille, -taille,   -taille, taille, -taille,   -taille, taille, taille,     // Face 5

						   -taille, taille, taille,   taille, taille, taille,   taille, taille, -taille,         // Face 6
						   -taille, taille, taille,   -taille, taille, -taille,   taille, taille, -taille};      // Face 6

		// Couleurs
		float colors[] = {1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1
                    1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1

                    0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2
                    0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2

                    0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3
                    0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3

                    1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4
                    1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4

                    0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5
                    0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5

                    0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 6
                    0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0};          // Face 6

	unsigned int indexes[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};

	Drawable cube(vertices,108,colors,108,indexes,36,"Shaders/couleur3D.vert","Shaders/couleur3D.frag");
	cube.load();

    // Shader
    Shader shaderCouleur("Shaders/couleur3D.vert", "Shaders/couleur3D.frag");
    shaderCouleur.load();


    // Matrices

    mat4 projection;
    mat4 modelview;

    projection = perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
    modelview = mat4(1.0);

	float angle = 0;

    // Boucle principale
    while(!terminer)
    {

        // Gestion des évènements
        SDL_PollEvent(&m_evenements);

        if(m_evenements.window.event == SDL_WINDOWEVENT_CLOSE)
            terminer = true;


        // Nettoyage de l'écran
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Réinitialisation de la matrice modelview
		modelview = lookAt(vec3(3, 3, 3), vec3(0, 0, 0), vec3(0, 1, 0));


		// Incrémentation de l'angle
		angle += 4.0;

		if(angle >= 360.0)
			angle -= 360.0;


		// Rotation du repère
		modelview = rotate(modelview, angle, vec3(0, 1, 0));

        // On spécifie quel shader utiliser
        /*glUseProgram(shaderCouleur.getProgramID());


            // On remplie puis on active le tableau Vertex Attrib 0

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
            glEnableVertexAttribArray(0);

            // Même chose avec le tableau Vertex Attrib 1
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, colors);
            glEnableVertexAttribArray(1);


            // On envoie les matrices au shader
            glUniformMatrix4fv(glGetUniformLocation(shaderCouleur.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
            glUniformMatrix4fv(glGetUniformLocation(shaderCouleur.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));


            // On affiche le polygone

            glDrawArrays(GL_TRIANGLES, 0, 36);


            // On désactive les tableaux Vertex Attrib puisque l'on n'en a plus besoin

            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);


        // On n'utilise plus le shader

        glUseProgram(0);*/

        cube.draw(modelview, projection);


        // Actualisation de la fenêtre

        SDL_GL_SwapWindow(m_fenetre);
    }
}
