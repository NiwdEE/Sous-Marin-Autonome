#ifndef DEF_OBJECT_TEXTURE
#define DEF_OBJECT_TEXTURE

#ifdef WIN32
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>


//static SDL_Surface* ReversePixels(SDL_Surface* src);

using namespace glm;

class Texture
{
    public:

        Texture();
        Texture(std::string path);
        Texture(Texture const&tocopy);
        Texture(int width, int height, GLenum format, GLenum internformat, bool isvoid);
        ~Texture();

        Texture&  operator=(Texture const &tocopy);

        void LoadVoidTexture(void);
        bool Load();

        GLuint getID() const;
        void setPath(const std::string &newpath);

        int getHeight();
        int getWidth();
        

    private:

        GLuint mID;
        std::string mPath;

        int mWidth;
        int mHeight;
        GLenum mFormat;
        GLenum mInternFormat;
        bool mVoidTexture;
};

namespace Txtr
{
    class Text
    {
        public:
            
            Text(std::string text, std::string font, int size, vec4 color);
            ~Text();

            bool Load();
            GLuint getID() const;

            void setText(std::string path);
            void setFont(std::string path);
            void setColor(vec4);

            int getHeight();
            int getWidth();


        private:
            
            GLuint mID;

            std::string mText;
            std::string mFont;
            int mTextSize;
            vec4 mColor;

            int mTextureWidth;
            int mTextureHeight;

            int mTextWidth;
            int mTextHeight;

            bool mLoaded;
        
    };
}

namespace Texture{

    class Texture
    {
        public:

            Texture();
            Texture(int width, int height, GLenum format, GLenum internformat, bool isvoid);
            Texture(const &Texture);
            ~Texture();

            bool Load();

            bool LoadVoidTexture();

            GLuint getID();
            int getHeight();
            int getWidth();
            bool isLoaded();

        protected:

            GLuint mID;

            int mHeight;
            int mWidth;
            
            GLenum mFormat;
            GLenum mInternFormat;

            bool mIsVoid;

            bool mLoaded;

        private:

        
    };

    class Image : public Texture
    {
        public:

            Image();
            Image(std::string mPath);
            Image(const &Image);
            ~Image();

            void setPath(std::string);

            bool Load();

        private:

            std::string mPath;
        
    };

    class Text : public Texture
    {
        public:

            Text();
            Text(std::string Text);
            Text(std::string Text, int Size);
            Text(std::string Text, int Size, std::string Font);
            Text(std::string Text, int Size, std::string Font, vec4 Color);
            Text(std::string Text, int Size, std::string Font, Uint32 Color);
            Text(std::string Text, int Size, vec4 Color);
            Text(std::string Text, int Size, Uint32 Color);
            Text(const &Text);
            ~Text();

            void setText(std::string);
            void setSize(int);
            void setFont(std::string);
            void setColor(vec4);
            void setColor(Uint32);
            void setColor(SDL_Color);

            bool Load();
        
        private:

            std::string mText;
            int mTextSize;
            std::string mFontPath;
            SDL_Color mColor;
    };
}

#endif