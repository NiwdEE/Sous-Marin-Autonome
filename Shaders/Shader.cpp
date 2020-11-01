#include "Shader.h"

Shader::Shader(void) : mVertexID(0), mFragmentID(0), mProgramID(0), mVertexSrc(), mFragmentSrc()
{

}

Shader::Shader(std::string vertexSource, std::string fragmentSource) : mVertexID(0), mFragmentID(0), mProgramID(0), mVertexSrc(vertexSource), mFragmentSrc(fragmentSource)
{

}

Shader::~Shader()
{
    glDeleteShader(mVertexID);
    glDeleteShader(mFragmentID);
    glDeleteProgram(mProgramID);
}

bool Shader::Load(void)
{

    if(glIsShader(mVertexID) == GL_TRUE)
        glDeleteShader(mVertexID);

    if(glIsShader(mFragmentID) == GL_TRUE)
        glDeleteShader(mFragmentID);

    if(glIsProgram(mProgramID) == GL_TRUE)
        glDeleteProgram(mProgramID);

    //Compilation + vérification des shaders

    if(!compileShader(mVertexID, GL_VERTEX_SHADER, mVertexSrc))
        return false;

    if(!compileShader(mFragmentID, GL_FRAGMENT_SHADER, mFragmentSrc))
        return false;

    //Création du programme
    mProgramID = glCreateProgram();

    //Association des shaders
    glAttachShader(mProgramID, mVertexID);
    glAttachShader(mProgramID, mFragmentID);

    // Verrouillage des entrées shader
    glBindAttribLocation(mProgramID, 0, "in_Vertex");
    glBindAttribLocation(mProgramID, 1, "in_Color");
    glBindAttribLocation(mProgramID, 2, "in_TexCoord0");
    glBindAttribLocation(mProgramID, 3, "in_Normals");

    //Linkage du programme
    glLinkProgram(mProgramID);

    GLint linkError(0);
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &linkError);

    if(linkError != GL_TRUE)
    {
        // Récupération de la taille de l'erreur

        GLint errorSize(0);
        glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &errorSize);


        // Allocation de mémoire

        char *error = new char[errorSize + 1];


        // Récupération de l'erreur

        glGetShaderInfoLog(mProgramID, errorSize, &errorSize, error);
        error[errorSize] = '\0';


        // Affichage de l'erreur

        std::cout << error << std::endl;


        // Libération de la mémoire et retour du booléen false

        delete[] error;
        glDeleteProgram(mProgramID);

        return false;
    }

    return true;
}

GLuint Shader::getProgramID(void) const
{
    return mProgramID;
}

void Shader::sendMat4(std::string uniformName, mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(mProgramID, uniformName.c_str()), 1 , GL_FALSE, &mat[0][0]);
}


bool Shader::compileShader(GLuint &shader, GLenum type, std::string const &Src)
{
    //Test de la validité du type + Création du shader
    if(!(shader = glCreateShader(type)))
    {
        std::cout << "Erreur Compilation Shader :\nLe type de shader " << type << " n'existe pas" << std::endl;
        return false;
    }

    //Test de validité + Ouverture du fichier source
    std::ifstream file(Src.c_str());
    if(!file) 
    {
        std::cout << "Erreur Compilation Shader :\nLe fichier de shader " << Src << " est introuvable" << std::endl;
        glDeleteShader(shader);
        return false;
    }

    //Récupération du code source
    std::string ligne;
    std::string sourceCode;

    while(getline(file, ligne))
        sourceCode += ligne + '\n';

    file.close();

    //Compilation du shader
    const GLchar* str_sourceCode = sourceCode.c_str();
    glShaderSource(shader, 1, &str_sourceCode, 0);
    glCompileShader(shader);

    //Vérification de la compilation

    GLint compileError{0};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileError);

    if(compileError!=GL_TRUE)
    {
        //Récupération de la taille des logs
        GLint errorSize{0};
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorSize);

        //Récupération des logs
        char* error = new char[errorSize + 1];
        glGetShaderInfoLog(shader, errorSize, &errorSize, error);
        error[errorSize] = '\0';

        //Affichage des logs
        std::cout << error << std::endl;
        delete[] error;
        glDeleteShader(shader);

        return false;
    }



    return true;
}