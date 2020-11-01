#include "texture2.hpp"

static SDL_Surface* ReversePixels(SDL_Surface* src)
{
    SDL_Surface* Reversed = SDL_CreateRGBSurface(0, src->w, src->h, src->format->BitsPerPixel, src->format->Rmask, src->format->Gmask, src->format->Bmask, src->format->Amask );
    unsigned char* pixelsSrc = (unsigned char*) src->pixels;
    unsigned char* pixelsInv = (unsigned char*) Reversed->pixels;

    for(int i = 0; i < src->h; i++)
    {
        for(int j = 0; j < src->w * src->format->BytesPerPixel; j++)
            pixelsInv[(src->w * src->format->BytesPerPixel * (src->h - 1 - i)) + j] = pixelsSrc[(src->w * src->format->BytesPerPixel * i) + j];
    }

    return Reversed;

}

Texture::Texture() : mID(0), mPath(""), mWidth(0), mHeight(0), mFormat(0), mInternFormat(0), mVoidTexture(0)
{

}

Texture::Texture(std::string path) : mID(0), mPath(path), mWidth(0), mHeight(0), mFormat(0), mInternFormat(0), mVoidTexture(0)
{

}

Texture::Texture(int width, int height, GLenum format, GLenum internformat, bool isvoid) : mID(0), mPath(""), mWidth(width), mHeight(height), mFormat(format), mInternFormat(internformat), mVoidTexture(isvoid)
{

}

Texture::Texture(Texture const&tocopy)
{
    mPath = tocopy.mPath;

    mWidth = tocopy.mWidth;
    mHeight = tocopy.mHeight;
    mFormat = tocopy.mFormat;
    mInternFormat = tocopy.mInternFormat;
    mVoidTexture = tocopy.mVoidTexture;

    if(glIsTexture(tocopy.mID)){
        if(mVoidTexture)
            this->LoadVoidTexture();
        else
            this->Load();
    }
}

Texture& Texture::operator=(Texture const &tocopy){

    mPath = tocopy.mPath;

    mWidth = tocopy.mWidth;
    mHeight = tocopy.mHeight;
    mFormat = tocopy.mFormat;
    mInternFormat = tocopy.mInternFormat;
    mVoidTexture = tocopy.mVoidTexture;

    if(glIsTexture(tocopy.mID)){
        if(mVoidTexture)
            this->LoadVoidTexture();
        else
            this->Load();
    }

    return *this;
}

void Texture::LoadVoidTexture(void)
{
    if(glIsTexture(mID) == GL_TRUE)
        glDeleteTextures(1, &mID);

    glGenTextures(1, &mID);

    glBindTexture(GL_TEXTURE_2D, mID);

        glTexImage2D(GL_TEXTURE_2D, 0, mInternFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

}

int Texture::getHeight()
{
    return mHeight;
}

int Texture::getWidth()
{
    return mWidth;
}

Texture::~Texture()
{

    glDeleteTextures(1, &mID);
}

bool Texture::Load(void)
{
    SDL_Surface* img_r = IMG_Load(mPath.c_str());
    if(!img_r){
        std::cout << "Erreur lors de la creation de la surface : " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Surface* img = ReversePixels(img_r);
    SDL_FreeSurface(img_r);

    if(glIsTexture(mID)==GL_TRUE)
        glDeleteTextures(1, &mID);

    glGenTextures(1, &mID);

    glBindTexture(GL_TEXTURE_2D, mID);

        GLenum ImgFormat{0};
        GLenum ClrFormat{0};

        if(img->format->BytesPerPixel == 3){
            ImgFormat = GL_RGB;

            if(img->format->Rmask == 0xff)
                ClrFormat = GL_RGB;
            else
                ClrFormat = GL_BGR;
        }
        else if(img->format->BytesPerPixel == 4){
            ImgFormat = GL_RGBA;

            if(img->format->Rmask == 0xff)
                ClrFormat = GL_RGBA;
            else
                ClrFormat = GL_BGRA;
        }
        else{
            std::cout << "Erreur : FI de l'image inconnu !" << std::endl;

            SDL_FreeSurface(img);
            return false;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, ImgFormat, img->w, img->h, 0, ClrFormat, GL_UNSIGNED_BYTE, img->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(img);

    return true;
}

GLuint Texture::getID() const
{
    return mID;
}

void Texture::setPath(const std::string &newpath)
{
    mPath = newpath;
}



Txtr::Text::Text(std::string text, std::string dfont, int size, vec4 color) : mID(0), mText(text), mFont(dfont), mTextSize(size), mColor(color), mTextureWidth(0), mTextureHeight(0), mTextWidth(0), mTextHeight(0), mLoaded(false)
{

}

Txtr::Text::~Text()
{
    glDeleteTextures(1, &mID);
}


bool Txtr::Text::Load()
{
    SDL_Color color = {Uint8(mColor.x), Uint8(mColor.y), Uint8(mColor.z), Uint8(mColor.w)};
    // SDL_Color color = {255, 255, 255 ,255 };

    TTF_Font* font = TTF_OpenFont(mFont.c_str(),mTextSize);
    if(font == NULL){
        printf("Erreur : Chargement de police échoué > %s\n", TTF_GetError());
    }

    SDL_Surface* text_r = TTF_RenderText_Blended(font, mText.c_str(), color);
    if(!text_r){
        printf("Erreur Affichage du texte\n");
    }

    //SDL_SaveBMP(text_r, "save.bmp");

    SDL_Surface* text = ReversePixels(text_r);
    SDL_FreeSurface(text_r);

    mTextureHeight = text -> h;
    mTextureWidth = text -> w;

    TTF_CloseFont(font);

    if(glIsTexture(mID))
        glDeleteTextures(1, &mID);

    glGenTextures(1, &mID);

    glBindTexture(GL_TEXTURE_2D, mID);

        GLenum ImgFormat{0};
        GLenum ClrFormat{0};

        if(text->format->BytesPerPixel == 3){
            ImgFormat = GL_RGB;

            if(text->format->Rmask == 0xff)
                ClrFormat = GL_RGB;
            else
                ClrFormat = GL_BGR;
        }
        else if(text->format->BytesPerPixel == 4){
            ImgFormat = GL_RGBA;

            if(text->format->Rmask == 0xff)
                ClrFormat = GL_RGBA;
            else
                ClrFormat = GL_BGRA;
        }
        else{
            std::cout << "Erreur : FI de l'image inconnu !" << std::endl;

            SDL_FreeSurface(text);
            return false;
        }

        //std::cout << "BPP : " << int(text->format->BytesPerPixel) << "; Rmask(" << int(0xff) << "): " << int(text->format->Rmask) << std::endl;

        glTexImage2D(GL_TEXTURE_2D, 0, ImgFormat, text->w, text->h, 0, ClrFormat, GL_UNSIGNED_BYTE, text->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(text);

    return true;
}

GLuint Txtr::Text::getID() const
{
    return mID;
}


void Txtr::Text::setText(std::string path)
{

}

void Txtr::Text::setFont(std::string path)
{

}

int Txtr::Text::getHeight()
{
    return mTextureHeight;
}

int Txtr::Text::getWidth()
{
    return mTextureWidth;
}





/*void Texture::Texture::LoadVoidTexture(void)
{
    if(glIsTexture(mID) == GL_TRUE)
        glDeleteTextures(1, &mID);

    glGenTextures(1, &mID);

    glBindTexture(GL_TEXTURE_2D, mID);

        glTexImage2D(GL_TEXTURE_2D, 0, mInternFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

}*/