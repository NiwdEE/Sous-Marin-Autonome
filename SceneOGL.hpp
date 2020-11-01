#ifndef DEF_OBJECT_SCENEOGL
#define DEF_OBJECT_SCENEOGL

#ifdef WIN32
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include <iostream>
#include <string>

#include "functions.h"
#include "texture.hpp"
#include "input.hpp"
#include "camera.hpp"
#include "shapes.hpp"
#include "framebuffer.hpp"
#include "player.hpp"
#include "physic.hpp"
#include "3Dobj.hpp"
#include "HUD.h"
#include "config.h"
#include <random>
#include <time.h>

#include "Shaders/Shader.h"

class SceneOGL
{
    public:

        SceneOGL(std::string WinTitle);
        ~SceneOGL();

        bool InitSDL(void);
        bool InitGL(void);
        void MainLoop(void);



    private:

        std::string mWinTitle;
        int mWinWidth;
        int mWinHeight;

        SDL_Window* mWindow;
        SDL_GLContext mContext;	

        Input mInput;
};

class Cube
{
    public:
        Cube(float taille, std::string const vertexShader, std::string const fragmentShader);
        ~Cube();

        void Load(void);


        void afficher(glm::mat4 &projection, glm::mat4 &modelview);

    private:
        Shader m_shader;
        float m_vertices[108];
        float m_couleurs[108];

        GLuint mVboID;
        GLuint mVaoID;
};

class Ground
{
    public:
        Ground(float taille, float reps, std::string const vertexShader, std::string const fragmentShader, std::string texturepath);
        ~Ground();
        void afficher(glm::mat4 &projection, glm::mat4 &modelview);

    private:
        Shader mShader;
        std::string mTexturePath;
        float mVerticeGround[18];
        float mVerticeTexture[12];
    
};


#endif