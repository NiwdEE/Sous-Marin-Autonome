#ifndef DEF_SHADER
#define DEF_SHADER

#ifdef WIN32
#include <GL/glew.h>



#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>


#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// Classe Shader

class Shader
{
    public:

        Shader();
        Shader(std::string vertexSource, std::string fragmentSource);
        ~Shader();

        bool Load(void);
        void sendMat4(std::string uniformName, mat4 mat);
        GLuint getProgramID(void) const;

    private:

        GLuint mVertexID;
        GLuint mFragmentID;
        GLuint mProgramID;

        std::string mVertexSrc;
        std::string mFragmentSrc;

        bool compileShader(GLuint &shader, GLenum type, std::string const &Src);
};



#endif