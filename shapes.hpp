#ifdef WIN32
#include <GL/glew.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#ifndef DEF_OBJECT_SHAPES
#define DEF_OBJECT_SHAPES

#include <iostream>
#include <cstring>
#include <boost/variant.hpp>
#include <boost/function.hpp>

#include "Shaders/shader.h"
#include "functions.h"
#include "texture.hpp"
#include "framebuffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define D(A,B) A B 0,001

using namespace glm;

enum RELABS
{
    REL,
    ABS
};




class Slab
{
    public:

        Slab(std::string const vertexShader, std::string const fragmentShader);
        Slab(vec3 pos, vec3 dim,std::string const vertexShader, std::string const fragmentShader);
        Slab(Slab const &tocopy);
        ~Slab();

        void Load(void);
        void UpdateVBO();

        Slab& operator=(Slab const& tocopy);

        void Display(mat4 &projection, glm::mat4 &modelview);

        void setPos(vec3 pos);
        template<typename T1,typename T2,typename T3>
        void setPos(T1 x, T2 y, T3 z);
        vec3 getPos(void) const;

        void setDim(vec3 pos);
        template<typename T1,typename T2,typename T3>
        void setDim(T1 x, T2 y, T3 z);
        vec3 getDim(void) const; 

        void setCenterPos(vec3 pos);
        void setCenterPos(RELABS ra, vec3 cpos);
        template<typename T1,typename T2,typename T3>
        void setCenterPos(T1 x, T2 y, T3 z);
        vec3 getCenterPos(void) const;

        void setAngles(vec2 dim);

        template<typename T1,typename T2>
        void setAngles(T1 x, T2 y);

        vec2 getAngles(void) const;

        void setTexture(std::string txtrpath);

        void setTextureReps(vec3 reps);
        
    private:

        vec3 mPos;
        vec3 mDims;
        vec2 mAngles;

        Shader mShader;
        float mVertex[108];
        float mTextureCoords[72];
        std::string mTexturePath;

        vec3 mPosCenter;

        GLuint mVboId;
        GLuint mVaoId;

        int mVertexSizeByte;
        int mTextureSizeByte;

        vec3 mTxtrSize;
        Texture mTexture;

        bool mLoaded;

        void refreshVertex();
        void refreshVertex(vec3 pos, vec3 posc, vec3 dim, vec2 angles);
};

class Rect
{
    public:
        
        Rect();
        Rect(std::string const vertexShader, std::string const fragmentShader);
        Rect(vec3 pos, vec2 dim, std::string const vertexShader, std::string const fragmentShader);
        ~Rect();

        void Load(void);
        void UpdateVBO();

        Slab& operator=(Slab const& tocopy);

        void Display(mat4 &projection, glm::mat4 &modelview);

        void setPos(vec3 pos);
        template<typename T1,typename T2,typename T3>
        void setPos(T1 x, T2 y, T3 z);
        vec3 getPos(void) const;

        void setDim(vec2 pos);
        template<typename T1,typename T2,typename T3>
        void setDim(T1 x, T2 y, T3 z);
        vec2 getDim(void) const; 

        void setCenterPos(vec3 pos);
        void setCenterPos(RELABS ra, vec3 cpos);
        template<typename T1,typename T2,typename T3>
        void setCenterPos(T1 x, T2 y, T3 z);
        vec3 getCenterPos(void) const;

        void setAngles(vec2 dim);

        template<typename T1,typename T2>
        void setAngles(T1 x, T2 y);

        vec2 getAngles(void) const;

        void setTexture(std::string txtrpath);

        void setTextureReps(vec3 reps);

    protected:

        Shader mShader;
        
        vec3 mPos;
        vec2 mDims;
        vec2 mAngles;

        float mVertex[18];

        Texture mTexture;

        vec3 mPosCenter;

        GLuint mVboId;
        GLuint mVaoId;

        bool mLoaded;

        void refreshVertex();
        void refreshVertex(vec3 pos, vec3 posc, vec2 dim, vec2 angles);

        float mTextureCoords[12];

        int mVertexSizeByte;
        int mTextureSizeByte;

    private:

        vec3 mTxtrSize;
        std::string mTexturePath;

};

#endif