#ifndef DEF_OBJECT_PLAYER
#define DEF_OBJECT_PLAYER


#ifdef WIN32
#include <GL/glew.h>
#endif

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "camera.hpp"
#include "input.hpp"
#include "physic.hpp"
#include "functions.h"

using namespace glm;

#define FRONT 0b0001
#define BACK  0b0010
#define RIGHT 0b0100
#define LEFT  0b1000



enum XYZ
{
    X,
    Y,
    Z
};

class Player
{
    public:

        Player();
        Player(uint id);
        Player(uint id, std::string name);
        Player(uint id, std::string name, float height);
        Player(uint id, std::string name, float height, Camera& mCamera);
        ~Player();

        void setSolids(std::map<std::string, vec3[3]> NO);
        void addSolid(std::string name, vec3 p1, vec3 p2, vec3 p3);
        void delSolid(std::string name);
        void clearSolids(void);

        void Refresh(void);
        void Jump(float);

        void setDirection(int);
        void setPos(vec3);

        void setVel(vec3);
        vec3 getVel(void);
        void addVel(vec3);

        void setAcc(vec3);
        void setAcc(XYZ, float);
        void setAcc(XYZ, float, XYZ, float);
        void addAcc(vec3);
        vec3 getAcc(void);

        void setHeight(float);
        float getHeight(void);

        bool IsOnGround(void);
        Uint32 OnGroundSince(void);
        int Moving(void);
        

    private:

        uint mID;
        std::string mName;

        float mHeight;
        int mWidth;
        Camera* mCamera;

        vec3 mPosition;
        vec3 mVelocity;
        vec3 mAcceleration;

        std::map<std::string, vec3[3]> mSolidPlanes;

        Uint32 mLastMoved;
        bool mOnGround;
        Uint32 mOnGroundSince;

        int mMoving;
        float mMovementSpeed;
        vec3 mMovementVelocity;
};


#endif