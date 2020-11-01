#include "shapes.hpp"


Slab::Slab(std::string const vertexShader, std::string const fragmentShader) : mPos(0,0,0), mDims(0,0,0), mAngles(0,0), mTexturePath("src/Textures/default.jpg"), mPosCenter(0,0,0),  
										 mVboId(0), mVaoId(0), mVertexSizeByte(108*sizeof(float)), mTextureSizeByte(72*sizeof(float)), mTxtrSize(1,1,1), mLoaded(false)
{
	mShader={vertexShader, fragmentShader};

	mShader.Load();

	for (int i = 0; i < 108; ++i) mVertex[i] = 0.0f;
}

Slab::Slab(vec3 pos, vec3 dim, std::string const vertexShader, std::string const fragmentShader) : mPos(pos), mDims(dim), mAngles(0,0), mTexturePath("src/Textures/default.jpg"), mPosCenter(0,0,0),  
										mVboId(0), mVaoId(0), mVertexSizeByte(108*sizeof(float)), mTextureSizeByte(72*sizeof(float)), mTxtrSize(1,1,1), mLoaded(false)
{
	mLoaded = false;
	mShader={vertexShader, fragmentShader};

	mShader.Load();

	refreshVertex();
}

Slab::Slab(Slab const &tocopy)
{
	mLoaded = false;
	mPos = tocopy.mPos;
	mDims = tocopy.mDims;
	mPosCenter = tocopy.mPosCenter;

	this->refreshVertex();
}


Slab::~Slab()
{
	if(glIsBuffer(mVboId) == GL_TRUE)
		glDeleteBuffers(1, &mVboId);

	if(glIsVertexArray(mVaoId) == GL_TRUE)
		glDeleteVertexArrays(1, &mVaoId);
}


Slab& Slab::operator = (Slab const &tocopy)
{
	mPos = tocopy.mPos;
	mDims = tocopy.mDims;
	mPosCenter = tocopy.mPosCenter;

	this->refreshVertex();

	return *this;
}

void Slab::Load()
{

	if(glIsBuffer(mVboId) == GL_TRUE)
        glDeleteBuffers(1, &mVboId);

	glGenBuffers(1,&mVboId);

	glBindBuffer(GL_ARRAY_BUFFER, mVboId);

		glBufferData(GL_ARRAY_BUFFER, mVertexSizeByte+mTextureSizeByte, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexSizeByte, mVertex);
		glBufferSubData(GL_ARRAY_BUFFER, mVertexSizeByte, mTextureSizeByte, mTextureCoords);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	if(glIsVertexArray(mVaoId) == GL_TRUE)
		glDeleteVertexArrays(1, &mVaoId);

	glGenVertexArrays(1, &mVaoId);

	glBindVertexArray(mVaoId);

		glBindBuffer(GL_ARRAY_BUFFER, mVboId);

	        // Envoi des vertices de surface

	        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	        glEnableVertexAttribArray(0);


	        // Envoi des vertices de texture

	        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mVertexSizeByte));
	        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	Texture txtr(mTexturePath);
    mTexture = txtr;
    mTexture.Load();



	mLoaded = true;
}

void Slab::UpdateVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVboId);

	void* vboptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if(vboptr==NULL)
	{
		std::cout << "Erreur de récupération d'adresse du VBO" << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return; 
	}

    memcpy((char*)vboptr, mVertex, mVertexSizeByte);
    memcpy((char*)vboptr + mVertexSizeByte, mTextureCoords, mTextureSizeByte);

    glUnmapBuffer(GL_ARRAY_BUFFER);
    vboptr = 0;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Slab::Display(mat4 &projection, mat4 &modelview)
{
	glUseProgram(mShader.getProgramID());

        glBindVertexArray(mVaoId);

	        mShader.sendMat4("modelviewProjection", projection * modelview);

	        glBindTexture(GL_TEXTURE_2D, mTexture.getID());

	        // Rendu

	        glDrawArrays(GL_TRIANGLES, 0, 36);

	        glBindTexture(GL_TEXTURE_2D, 0);

	        // Désactivation des tableaux

	        // glDisableVertexAttribArray(2);
	        // glDisableVertexAttribArray(0);

        glBindVertexArray(0);

    // Désactivation du shader

    glUseProgram(0);
}

void Slab::refreshVertex()
{
	refreshVertex(mPos, mPosCenter, mDims, mAngles);
}

void Slab::refreshVertex(vec3 pos, vec3 posc, vec3 dim, vec2 angles)
{
	float phi = M_PI*angles.x/180;
	float theta = M_PI*angles.y/180;

	float TmpVertex[108] = {

	//	                  Triangle 1                                                        Triangle 2
	//	    P1               P2                 P3                         P1                   P2                 P3 
	//    x y z            x y z              x y z                      x y z                 x y z              x y z         
		  0,0,0,         dim.x,0,0,         0,0,dim.z,      /**/      dim.x,0,dim.z,         dim.x,0,0,         0,0,dim.z,    //Face Bas
		  0,0,0,         dim.x,0,0,         0,dim.y,0,      /**/      dim.x,dim.y,0,         dim.x,0,0,         0,dim.y,0,    //Face Arrière
		  0,0,0,         0,0,dim.z,         0,dim.y,0,      /**/      0,dim.y,dim.z,         0,0,dim.z,         0,dim.y,0,    //Face Gauche
		dim.x,0,0,     dim.x,0,dim.z,     dim.x,dim.y,0,    /**/    dim.x,dim.y,dim.z,     dim.x,0,dim.z,     dim.x,dim.y,0,  //Face Droite
		0,0,dim.z,     dim.x,0,dim.z,     0,dim.y,dim.z,    /**/    dim.x,dim.y,dim.z,     dim.x,0,dim.z,     0,dim.y,dim.z,  //Face Avant 
		0,dim.y,0,     dim.x,dim.y,0,     0,dim.y,dim.z,    /**/    dim.x,dim.y,dim.z,     dim.x,dim.y,0,     0,dim.y,dim.z   //Face Haut

	};

	float coordTexture[72] = {

		0, 0,   mTxtrSize.x, 0,   0, mTxtrSize.z,     mTxtrSize.x, mTxtrSize.z,   mTxtrSize.x, 0,   0, mTxtrSize.z, //Face Bas
        0, 0,   mTxtrSize.x, 0,   0, mTxtrSize.y,     mTxtrSize.x, mTxtrSize.y,   mTxtrSize.x, 0,   0, mTxtrSize.y, //Face Arrière
        0, 0,   mTxtrSize.z, 0,   0, mTxtrSize.y,     mTxtrSize.z, mTxtrSize.y,   mTxtrSize.z, 0,   0, mTxtrSize.y, //Face Gauche
        0, 0,   mTxtrSize.z, 0,   0, mTxtrSize.y,     mTxtrSize.z, mTxtrSize.y,   mTxtrSize.z, 0,   0, mTxtrSize.y, //Face Droite
        0, 0,   mTxtrSize.x, 0,   0, mTxtrSize.y,     mTxtrSize.x, mTxtrSize.y,   mTxtrSize.x, 0,   0, mTxtrSize.y, //Face Avant 
        0, 0,   mTxtrSize.x, 0,   0, mTxtrSize.z,     mTxtrSize.x, mTxtrSize.z,   mTxtrSize.x, 0,   0, mTxtrSize.z  //Face Haut

    }; 

    float placeonorigin[3] = {dim.x/2,dim.y/2, dim.z/2};
    for (int i = 0; i < 108; ++i)
    	TmpVertex[i] -= placeonorigin[i%3];

    if(phi)
    for (int i = 0; i < 108; i+=3)
    {
    	float xprime = TmpVertex[i]*cos(phi) - TmpVertex[i+1]*sin(phi);
    	float yprime = TmpVertex[i+1]*cos(phi) + TmpVertex[i]*sin(phi);
    	TmpVertex[i] = xprime;
    	TmpVertex[i+1] = yprime;
    }

    if(theta)
    for (int i = 0; i < 108; i+=3)
    {
    	float xprime = TmpVertex[i]*cos(theta) + TmpVertex[i+2]*sin(theta);
    	float zprime = TmpVertex[i+2]*cos(theta) - TmpVertex[i]*sin(theta);
    	TmpVertex[i] = xprime;
    	TmpVertex[i+2] = zprime;
    }

	float toadd[3] = {pos.x-posc.x,pos.y-posc.y,pos.z-posc.z};
	for (int i = 0; i < 108; ++i)
		mVertex[i] = TmpVertex[i] + toadd[i%3] + placeonorigin[i%3];

	for (int i = 0; i < 72; ++i)
		mTextureCoords[i] = coordTexture[i];

	if (mLoaded)
		this->UpdateVBO();
}

void Slab::setPos(vec3 pos)
{
	mPos = pos;
	this->refreshVertex();
}

template<typename T1,typename T2,typename T3>
void Slab::setPos(T1 x, T2 y, T3 z)
{
	mPos = vec3(float(x),float(y),float(z));
	this->refreshVertex();
}

vec3 Slab::getPos(void) const
{
	return mPos;
}

void Slab::setDim(vec3 dim)
{
	mDims = dim;
	this->refreshVertex();
}

template<typename T1,typename T2,typename T3>
void Slab::setDim(T1 x, T2 y, T3 z)
{
	mDims = vec3(float(x),float(y),float(z));
	this->refreshVertex();
}

vec3 Slab::getDim(void) const
{
	return mDims;
}

void Slab::setCenterPos(vec3 cpos)
{
	mPosCenter = cpos;
	this->refreshVertex();
}

void Slab::setCenterPos(RELABS ra, vec3 cpos)
{
	if(ra == REL)
		mPosCenter = cpos * mDims;
	else
		mPosCenter = cpos;

	this->refreshVertex();
}

template<typename T1,typename T2,typename T3>
void Slab::setCenterPos(T1 x, T2 y, T3 z)
{
	mPosCenter = vec3(float(x),float(y),float(z));
	this->refreshVertex();
}


vec3 Slab::getCenterPos(void) const
{
	return mPosCenter;
}

void Slab::setAngles(vec2 dim)
{
	mAngles = dim;
	this->refreshVertex();
}

template<typename T1,typename T2>
void Slab::setAngles(T1 x, T2 y)
{
	mAngles = vec2(float(x),float(y));
	this->refreshVertex();
}

vec2 Slab::getAngles(void) const
{
	return mAngles;
}

void Slab::setTexture(std::string txtrpath)
{
	mTexturePath = txtrpath;
}

void Slab::setTextureReps(vec3 reps)
{
	mTxtrSize = reps;

	this->refreshVertex();
}

Rect::Rect() : mShader("Shaders/texture.vert","Shaders/texture.frag"), mPos(0,0,0), mDims(0,0), mAngles(0,0), mPosCenter(0,0,0), mVboId(0), mVaoId(0), mLoaded(false),
			mVertexSizeByte(18*sizeof(float)), mTextureSizeByte(12*sizeof(float)), mTxtrSize(1,1,1), mTexturePath("src/Textures/default.jpg")
{
	mShader.Load();
}


Rect::Rect(std::string const vertexShader, std::string const fragmentShader) : mPos(0,0,0), mDims(0,0), mAngles(0,0), mPosCenter(0,0,0), mVboId(0), mVaoId(0), mLoaded(false),
				mVertexSizeByte(18*sizeof(float)), mTextureSizeByte(12*sizeof(float)), mTxtrSize(1,1,1), mTexturePath("src/Textures/default.jpg")
{
	mShader={vertexShader, fragmentShader};

	mShader.Load();

	for (int i = 0; i < 18; ++i) mVertex[i] = 0.0f;
}

Rect::Rect(vec3 pos, vec2 dim, std::string const vertexShader, std::string const fragmentShader) : mPos(pos), mDims(dim), mAngles(0,0), mPosCenter(0,0,0), mVboId(0), mVaoId(0), mLoaded(false),
				mVertexSizeByte(18*sizeof(float)), mTextureSizeByte(12*sizeof(float)), mTxtrSize(1,1,1), mTexturePath("src/Textures/default.jpg")
{
	mShader={vertexShader, fragmentShader};

	mShader.Load();

	refreshVertex();
}


Rect::~Rect()
{
	if(glIsBuffer(mVboId) == GL_TRUE)
		glDeleteBuffers(1, &mVboId);

	if(glIsVertexArray(mVaoId) == GL_TRUE)
		glDeleteVertexArrays(1, &mVaoId);
}

void Rect::Load()
{

	if(glIsBuffer(mVboId) == GL_TRUE)
        glDeleteBuffers(1, &mVboId);

	glGenBuffers(1,&mVboId);

	glBindBuffer(GL_ARRAY_BUFFER, mVboId);

		glBufferData(GL_ARRAY_BUFFER, mVertexSizeByte + mTextureSizeByte, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexSizeByte, mVertex);
		glBufferSubData(GL_ARRAY_BUFFER, mVertexSizeByte, mTextureSizeByte, mTextureCoords);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	if(glIsVertexArray(mVaoId) == GL_TRUE)
		glDeleteVertexArrays(1, &mVaoId);

	glGenVertexArrays(1, &mVaoId);

	glBindVertexArray(mVaoId);

		glBindBuffer(GL_ARRAY_BUFFER, mVboId);

	        // Envoi des vertices de surface

	        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	        glEnableVertexAttribArray(0);


	        // Envoi des vertices de texture

	        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mVertexSizeByte));
	        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	Texture txtr(mTexturePath);
	mTexture = txtr;
	mTexture.Load();

	mLoaded = true;
}

void Rect::UpdateVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVboId);

	void* vboptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if(vboptr==NULL)
	{
		std::cout << "Erreur de récupération d'adresse du VBO" << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return; 
	}

    memcpy((char*)vboptr, mVertex, 18*sizeof(float));
    memcpy((char*)vboptr + 18*sizeof(float), mTextureCoords, 12*sizeof(float));

    glUnmapBuffer(GL_ARRAY_BUFFER);
    vboptr = 0;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Rect::Display(mat4 &projection, mat4 &modelview)
{
	glUseProgram(mShader.getProgramID());

        glBindVertexArray(mVaoId);

	        mShader.sendMat4("modelviewProjection", projection * modelview);

	        glBindTexture(GL_TEXTURE_2D, mTexture.getID());

	        // Rendu

	        glDrawArrays(GL_TRIANGLES, 0, 6);

	        glBindTexture(GL_TEXTURE_2D, 0);

	        // Désactivation des tableaux

	        // glDisableVertexAttribArray(2);
	        // glDisableVertexAttribArray(0);

        glBindVertexArray(0);

    // Désactivation du shader

    glUseProgram(0);
}

void Rect::refreshVertex()
{
	refreshVertex(mPos, mPosCenter, mDims, mAngles);
}

void Rect::refreshVertex(vec3 pos, vec3 posc, vec2 dim, vec2 angles)
{
	float phi = M_PI*angles.x/180;
	float theta = M_PI*angles.y/180;

	float TmpVertex[18] = {

	//	                  Triangle 1                                                        Triangle 2
	//	    P1               P2                 P3                         P1                   P2                 P3 
	//    x y z            x y z              x y z                      x y z                 x y z              x y z         
		  0,0,0,         dim.x,0,0,         0,dim.y,0,      /**/      dim.x,dim.y,0,         dim.x,0,0,         0,dim.y,0,    //Face Arrière

	};

	float coordTexture[12] = {
        0, 0,   mTxtrSize.x, 0,   0, mTxtrSize.y,     mTxtrSize.x, mTxtrSize.y,   mTxtrSize.x, 0,   0, mTxtrSize.y, //Face Arrière
    };

    float placeonorigin[3] = {dim.x/2,dim.y/2, 0};
    for (int i = 0; i < 18; ++i)
    	TmpVertex[i] -= placeonorigin[i%3];

    if(phi)
    for (int i = 0; i < 18; i+=3)
    {
    	float xprime = TmpVertex[i]*cos(phi) - TmpVertex[i+1]*sin(phi);
    	float yprime = TmpVertex[i+1]*cos(phi) + TmpVertex[i]*sin(phi);
    	TmpVertex[i] = xprime;
    	TmpVertex[i+1] = yprime;
    }

    if(theta)
    for (int i = 0; i < 18; i+=3)
    {
    	float xprime = TmpVertex[i]*cos(theta) + TmpVertex[i+2]*sin(theta);
    	float zprime = TmpVertex[i+2]*cos(theta) - TmpVertex[i]*sin(theta);
    	TmpVertex[i] = xprime;
    	TmpVertex[i+2] = zprime;
    }

	float toadd[3] = {pos.x-posc.x,pos.y-posc.y,pos.z-posc.z};
	for (int i = 0; i < 18; ++i)
		mVertex[i] = TmpVertex[i] + toadd[i%3] + placeonorigin[i%3];

	for (int i = 0; i < 12; ++i)
		mTextureCoords[i] = coordTexture[i];

	if (mLoaded)
		this->UpdateVBO();
}

void Rect::setPos(vec3 pos)
{
	mPos = pos;
	this->refreshVertex();
}

template<typename T1,typename T2,typename T3>
void Rect::setPos(T1 x, T2 y, T3 z)
{
	mPos = vec3(float(x),float(y),float(z));
	this->refreshVertex();
}

vec3 Rect::getPos(void) const
{
	return mPos;
}

void Rect::setDim(vec2 dim)
{
	mDims = dim;
	this->refreshVertex();
}

template<typename T1,typename T2,typename T3>
void Rect::setDim(T1 x, T2 y, T3 z)
{
	mDims = vec2(float(x),float(y));
	this->refreshVertex();
}

vec2 Rect::getDim(void) const
{
	return mDims;
}

void Rect::setCenterPos(vec3 cpos)
{
	mPosCenter = cpos;
	this->refreshVertex();
}

void Rect::setCenterPos(RELABS ra, vec3 cpos)
{
	if(ra == REL)
		mPosCenter = cpos.x * vec3(mDims, 1);
	else
		mPosCenter = cpos;

	this->refreshVertex();
}

template<typename T1,typename T2,typename T3>
void Rect::setCenterPos(T1 x, T2 y, T3 z)
{
	mPosCenter = vec3(float(x),float(y),float(z));
	this->refreshVertex();
}


vec3 Rect::getCenterPos(void) const
{
	return mPosCenter;
}

void Rect::setAngles(vec2 dim)
{
	mAngles = dim;
	this->refreshVertex();
}

template<typename T1,typename T2>
void Rect::setAngles(T1 x, T2 y)
{
	mAngles = vec2(float(x),float(y));
	this->refreshVertex();
}

vec2 Rect::getAngles(void) const
{
	return mAngles;
}

void Rect::setTexture(std::string txtrpath)
{
	mTexturePath = txtrpath;
}

void Rect::setTextureReps(vec3 reps)
{
	mTxtrSize = reps;

	this->refreshVertex();
}

