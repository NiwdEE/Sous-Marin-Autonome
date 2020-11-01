#ifndef DEF_3DOBJ
#define DEF_3DOBJ

#ifdef WIN32
#include <GL/glew.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "texture.hpp"
#include "Shaders/Shader.h"
#include "functions.h"


using namespace glm;

struct Material
{
    float Ns;
    float Ka[3];
    float Kd[3];
    float Ks[3];
    float Ke[3];
    float Ni;
    float d;
    int illum;

    std::string map_Kd;
    std::string map_d;
};

class Object3D
{
    public:

        Object3D(std::string const vertexShader, std::string const fragmentShader);
        Object3D(std::string const vertexShader, std::string const fragmentShader, std::string path);
        Object3D(std::string const vertexShader, std::string const fragmentShader, std::string path, float sizeMult);
        ~Object3D();

        bool Read();
        bool ReadMTL();

        void UpdateVBO();
        bool Load();
        void Display(mat4&, mat4&);

        

    private:

        std::string mPath;
        std::string mTexturePath;
        std::string mMaterialPath;

        float mSizeMult;

        bool mLoaded;
        bool mUsingMTL;

        std::vector<vec3> mVertex;
        std::vector<vec2> mUvs;
        std::vector<vec3> mNormals;

        Shader mShader;

        GLuint mVboID;
        GLuint mVaoID;

        Uint32 mVertexSizeByte;
        Uint32 mTextureSizeByte;
        Uint32 mNormalsSizeByte;

        Texture mTexture;
              
};


#endif

