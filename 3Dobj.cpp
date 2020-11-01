#include "3Dobj.hpp"

Object3D::Object3D(std::string const vertexShader, std::string const fragmentShader) : mPath(NULL), mSizeMult(0), mLoaded(false), mVboID(0), mVaoID(0)
{
    mShader={vertexShader, fragmentShader};

    mShader.Load();
}

Object3D::Object3D(std::string const vertexShader, std::string const fragmentShader, std::string path) : mPath(path), mSizeMult(0), mLoaded(false), mVboID(0), mVaoID(0)
{
    mShader={vertexShader, fragmentShader};

    mShader.Load();
}

Object3D::Object3D(std::string const vertexShader, std::string const fragmentShader, std::string path, float sizeMult) : mPath(path), mSizeMult(sizeMult), mLoaded(false), mVboID(0), mVaoID(0)
{
    mShader={vertexShader, fragmentShader};

    mShader.Load();
}

Object3D::~Object3D()
{

}


bool Object3D::Read()
{
    FILE* file = fopen(mPath.c_str(), "r");
    if(!file)
        return false;


    char LineHeader[128];
    char buffer[128];
    int res;

    vec3 vertex;
    vec2 uv;
    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<vec3> tmp_vertex, tmp_normal;
    std::vector<vec2> tmp_uv;

    int matches;
    unsigned int i = 0;

    while(1)
    {
        i++;

        res = fscanf(file, "%s", LineHeader);
        if(res == EOF)
            break;

        if(strcmp(LineHeader, "v") == 0){
            matches = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            if(matches != 3){
                std::cout << "Vertex Line No. " << i << " Couldn't be read" << std::endl;
                return false;
            }
            tmp_vertex.push_back(vertex);
        }else if(strcmp(LineHeader, "vt") == 0){
            matches = fscanf(file, "%f %f\n", &uv.x, &uv.y);
            if(matches != 2){
                std::cout << "Uv Coords Line No. " << i << " Couldn't be read" << std::endl;
                return false;
            }
            tmp_uv.push_back(uv);
        }else if(strcmp(LineHeader, "vn") == 0){
            matches = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            if(matches != 3){
                std::cout << "Normal vector Line No. " << i << " Couldn't be read" << std::endl;
                return false;
            }
            tmp_normal.push_back(vertex);
        }else if (strcmp(LineHeader, "f" ) == 0 ){
            matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if(matches != 9){
                std::cout << "Face Line No. " << i << " Couldn't be read" << std::endl;
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }else if(strcmp(LineHeader, "usemtl") == 0){
            matches = fscanf(file, "%s\n", buffer);
            if(matches != 1){
                std::cout << "Material part include Line No. " << i << " Couldn't be read" << std::endl;
                return false;
            }

        }else if(strcmp(LineHeader, "mtllib") == 0){
            matches = fscanf(file, "%s.mtl\n",  buffer);
            if(matches != 1){
                std::cout << "Material file include Line No. " << i << " Couldn't be read" << std::endl;
                return false;
            }
            mMaterialPath = std::string(buffer) /*+ std::string(".mtl")*/;
            // std::cout << mMaterialPath << std::endl;
        }
    }

    for (i = 0; i < vertexIndices.size(); ++i)
    {
        mVertex.push_back(tmp_vertex[vertexIndices[i] - 1]);
        mUvs.push_back(tmp_uv[uvIndices[i] - 1]);
        mNormals.push_back(tmp_normal[normalIndices[i] - 1]);

        //std::cout << mVertex[i] << std::endl;
    }

    mVertexSizeByte = mVertex.size() * sizeof(vec3);
    mTextureSizeByte = mUvs.size() * sizeof(vec2);
    mNormalsSizeByte = mNormals.size() * sizeof(vec3);



    return true;
}

bool Object3D::ReadMTL()
{
    return false;
}



bool Object3D::Load()
{
    if(!Read())
        return false;
    
    if(glIsBuffer(mVboID) == GL_TRUE)
        glDeleteBuffers(1, &mVboID);

    glGenBuffers(1,&mVboID);

    glBindBuffer(GL_ARRAY_BUFFER, mVboID);

        glBufferData(GL_ARRAY_BUFFER, mVertexSizeByte+mTextureSizeByte+mNormalsSizeByte, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexSizeByte, &mVertex[0]);
        glBufferSubData(GL_ARRAY_BUFFER, mVertexSizeByte, mTextureSizeByte, &mUvs[0]);
        glBufferSubData(GL_ARRAY_BUFFER, mVertexSizeByte + mTextureSizeByte, mNormalsSizeByte , &mNormals[0]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    if(glIsVertexArray(mVaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &mVaoID);

    glGenVertexArrays(1, &mVaoID);

    glBindVertexArray(mVaoID);

        glBindBuffer(GL_ARRAY_BUFFER, mVboID);

            // Envoi des vertices de surface

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);


            // Envoi des vertices de texture

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mVertexSizeByte));
            glEnableVertexAttribArray(2);

            // Envoi des normales des sommets

            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mVertexSizeByte + mTextureSizeByte));
            glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    Texture txtr("src/Textures/wood1.jpg");
    mTexture = txtr;
    mTexture.Load();



    mLoaded = true;
    return true;
}

void Object3D::UpdateVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, mVboID);

        void* vboptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        if(vboptr==NULL)
        {
            std::cout << "Erreur de récupération d'adresse du VBO" << std::endl;
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            return; 
        }

        memcpy((char*)vboptr, &mVertex[0], mVertexSizeByte);
        memcpy((char*)vboptr + mVertexSizeByte, &mUvs[0], mTextureSizeByte);
        memcpy((char*)vboptr + mVertexSizeByte + mTextureSizeByte, &mNormals[0], mNormalsSizeByte);

        glUnmapBuffer(GL_ARRAY_BUFFER);
        vboptr = 0;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object3D::Display(mat4 &projection, mat4 &modelview)
{
    if(!mLoaded)
        return;

    glUseProgram(mShader.getProgramID());

        glBindVertexArray(mVaoID);

            mShader.sendMat4("MVP", projection * modelview);
            mShader.sendMat4("M", mat4(1.0));
            mShader.sendMat4("V", modelview);
            glUniform3f(glGetUniformLocation(mShader.getProgramID(), "LightPosition_worldspace"), 0.0f, 0.0f, 10000000.0f);

            glBindTexture(GL_TEXTURE_2D, mTexture.getID());

                // Rendu

                glDrawArrays(GL_TRIANGLES, 0, mVertex.size());

            glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

    // Désactivation du shader

    glUseProgram(0);
}