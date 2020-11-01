#include "player.hpp"

Player::Player() : mID(0), mName("NULL"), mHeight(0), mWidth(1), mCamera(nullptr), mPosition(vec3(0,0,0)), mVelocity(vec3(0,0,0)),
                    mAcceleration(vec3(0,-9.81,0)), mLastMoved(0), mOnGround(false),mMoving(0), mMovementSpeed(100), mMovementVelocity(vec3(0,0,0))
{

}

Player::Player(uint id) : mID(id), mName("NULL"), mHeight(0), mWidth(1), mCamera(nullptr), mPosition(vec3(0,0,0)), mVelocity(vec3(0,0,0)),
                    mAcceleration(vec3(0,-9.81,0)), mLastMoved(0), mOnGround(false),mMoving(0), mMovementSpeed(100), mMovementVelocity(vec3(0,0,0))
{

}


Player::Player(uint id, std::string name) : mID(id), mName(name), mHeight(0), mWidth(1), mCamera(nullptr), mPosition(vec3(0,0,0)), mVelocity(vec3(0,0,0)),
                    mAcceleration(vec3(0,-9.81,0)), mLastMoved(0), mOnGround(false),mMoving(0), mMovementSpeed(100), mMovementVelocity(vec3(0,0,0))
{

}

Player::Player(uint id, std::string name, float height) : mID(id), mName(name), mHeight(height), mWidth(1), mCamera(nullptr), mPosition(vec3(0,0,0)), mVelocity(vec3(0,0,0)),
                    mAcceleration(vec3(0,-9.81,0)), mLastMoved(0), mOnGround(false),mMoving(0), mMovementSpeed(100), mMovementVelocity(vec3(0,0,0))
{

}

Player::Player(uint id, std::string name, float height, Camera& camera) : mID(id), mName(name), mHeight(height), mWidth(1), mCamera(&camera), mPosition(vec3(0,0,0)), mVelocity(vec3(0,0,0)),
                    mAcceleration(vec3(0,-9.81,0)), mLastMoved(0), mOnGround(false),mMoving(0), mMovementSpeed(100), mMovementVelocity(vec3(0,0,0))
{
    mLastMoved = SDL_GetTicks();
}




Player::~Player()
{
    mSolidPlanes.clear();
}


void Player::setSolids(std::map<std::string, vec3[3]> NO)
{
    mSolidPlanes.clear();
    mSolidPlanes = NO;
}

void Player::addSolid(std::string name, vec3 p1, vec3 p2, vec3 p3)
{
    //mSolidPlanes[name] = {p1,p2,p3};
}

void Player::delSolid(std::string name)
{
    mSolidPlanes.erase(name);
}

void Player::clearSolids(void)
{
    mSolidPlanes.clear();
}

void Player::setPos(vec3 NO)
{
    mPosition = NO;
}

void Player::setVel(vec3 NO)
{
    mVelocity = NO;
}

vec3 Player::getVel(void)
{
    return mVelocity;
}

void Player::addVel(vec3 NO)
{
    mVelocity += NO;
}

void Player::setAcc(vec3 NO)
{
    mAcceleration = NO;
}

void Player::setAcc(XYZ wich, float NO)
{
    if(wich == XYZ::X)
        mAcceleration.x = NO;
    else if(wich == XYZ::Z)
        mAcceleration.z = NO;
    else if(wich == XYZ::Y)
        mAcceleration.y = NO;
    else
        return;
}

void Player::setAcc(XYZ wich1, float NO1, XYZ wich2, float NO2)
{
    if(wich1 == XYZ::X)
        mAcceleration.x = NO1;
    else if(wich1 == XYZ::Z)
        mAcceleration.z = NO1;
    else if(wich1 == XYZ::Y)
        mAcceleration.y = NO1;
    else
        return;

    if(wich2 == XYZ::X)
        mAcceleration.x = NO2;
    else if(wich2 == XYZ::Z)
        mAcceleration.z = NO2;
    else if(wich2 == XYZ::Y)
        mAcceleration.y = NO2;
    else
        return;
}

void Player::addAcc(vec3 NO)
{
    mAcceleration += NO;
}

vec3 Player::getAcc(void)
{
    return mAcceleration;
}



void Player::Refresh(void)
{
    float LastMovedDeltaS =(SDL_GetTicks() - mLastMoved);

    float LastMovedDeltaS10P1 = 1 + LastMovedDeltaS / 100.0f;

    LastMovedDeltaS/=1000.0f;
    // vec3 NewPos = mPosition + LastMovedDeltaS*mVelocity;

    bool xBlockedP(false), yBlockedP(false), zBlockedP(false), xBlockedM(false), yBlockedM(false), zBlockedM(false);

    // for (std::map<std::string, vec3[3]>::iterator it=mSolidPlanes.begin(); it!=mSolidPlanes.end(); it++)
    // {
    //     if(!yBlockedM)
    //     {
            
    //     }
    // }

    if(mPosition.y<0)
        yBlockedM = true;
    else
        yBlockedM = false;

    if(yBlockedM != mOnGround && !mOnGround){
        mOnGroundSince = SDL_GetTicks();
        //printf("atteri\n");
    }

    // std::cout << "Début Acceleration -> Vitesse  " << SDL_GetTicks() << std::endl;

    if(mAcceleration.x)
        if((mAcceleration.x > 0 && !xBlockedP) || (mAcceleration.x < 0 && !xBlockedM))
            mVelocity.x += mAcceleration.x * LastMovedDeltaS;

    if(mAcceleration.y)
        if((mAcceleration.y > 0 && !yBlockedP) || (mAcceleration.y < 0 && !yBlockedM))
            mVelocity.y += mAcceleration.y * LastMovedDeltaS;

    if(mAcceleration.z)
        if((mAcceleration.z > 0 && !zBlockedP) || (mAcceleration.z < 0 && !zBlockedM))
            mVelocity.z += mAcceleration.z * LastMovedDeltaS;

    // std::cout << "Début Friction -> Vitesse      " << SDL_GetTicks() << std::endl;

    if (mMoving)
    {
        vec3 DirectionVector = normalize(vec3(mCamera->getOrientation().x,0,mCamera->getOrientation().z));
        vec3 LateralVector = normalize(vec3(mCamera->getLateral().x,0,mCamera->getLateral().z));
        vec3 FinalVector = vec3(0,0,0);

        if(bit(mMoving, 0)){
            FinalVector.x += DirectionVector.x;
            FinalVector.z += DirectionVector.z;
        }

        if(bit(mMoving, 1)){
            FinalVector.x += -DirectionVector.x;
            FinalVector.z += -DirectionVector.z;
        }

        if(bit(mMoving, 2)){
            FinalVector.x += LateralVector.x;
            FinalVector.z += LateralVector.z;
        }

        if(bit(mMoving, 3)){
            FinalVector.x += -LateralVector.x;
            FinalVector.z += -LateralVector.z;
        }

        FinalVector = normalize(FinalVector);

        if(!yBlockedM)
            FinalVector = 0.1f * FinalVector;

        mMovementVelocity.x += LastMovedDeltaS * mMovementSpeed * FinalVector.x;
        mMovementVelocity.z += LastMovedDeltaS * mMovementSpeed * FinalVector.z;
    }
    else if(yBlockedM)
    {
        if(mMovementVelocity.x > 0.05 || mMovementVelocity.x < -0.05)
            mMovementVelocity.x /= LastMovedDeltaS10P1;
        else
            mMovementVelocity.x = 0;

        if(mMovementVelocity.z > 0.05 || mMovementVelocity.z < -0.05)
            mMovementVelocity.z /= LastMovedDeltaS10P1;
        else
            mMovementVelocity.z = 0;
    }

    if(yBlockedM && mVelocity != vec3(0,0,0)){
        if(!mMoving){
            if(mVelocity.x > 0.1 || mVelocity.x < -0.1)
                mVelocity.x /= LastMovedDeltaS10P1;
            else
                mVelocity.x = 0;

            if(mVelocity.z > 0.1 || mVelocity.z < -0.1)
                mVelocity.z /= LastMovedDeltaS10P1;
            else
                mVelocity.z = 0;
        }

        if(mVelocity.y < 0)
            mVelocity.y = 0;

        if(mMovementVelocity.y < 0)
            mMovementVelocity.y = 0;
    }

    // std::cout << "Début Vitesse -> Position      " << SDL_GetTicks() << std::endl;

    if( mMovementVelocity.x*mMovementVelocity.x + mMovementVelocity.y*mMovementVelocity.y + mMovementVelocity.z*mMovementVelocity.z > 25)
        mMovementVelocity = 5.0f * normalize(mMovementVelocity);

    vec3 mTVelocity = mVelocity + mMovementVelocity;

    if((mTVelocity.y < 0 && !yBlockedM) || (mTVelocity.y > 0 && !yBlockedP))
        mPosition.y += (mTVelocity.y) * LastMovedDeltaS;

    if((mTVelocity.x < 0 && !xBlockedM) || (mTVelocity.x > 0 && !xBlockedP))
        mPosition.x += (mTVelocity.x) * LastMovedDeltaS;

    if((mTVelocity.z < 0 && !zBlockedM) || (mTVelocity.z > 0 && !zBlockedP))
        mPosition.z += (mTVelocity.z) * LastMovedDeltaS;

    // std::cout << "Fin                            " << SDL_GetTicks() << std::endl;

    mOnGround = yBlockedM;

    mLastMoved = SDL_GetTicks();

    mCamera->setPos(vec3(mPosition.x, mPosition.y + mHeight, mPosition.z));

    // std::cout << mVelocity << std::endl;
}

void Player::Jump(float YVelocity)
{
    if(mOnGround){
        //mOnGround = false;
        mVelocity.y = YVelocity;
    }

    // mVelocity += mMovementVelocity;
    // mMovementVelocity = vec3(0,0,0);
}

void Player::setDirection(int Direction)
{
    mMoving = Direction;
}

bool Player::IsOnGround(void)
{
    return mOnGround;
}

Uint32 Player::OnGroundSince(void)
{
    return mOnGroundSince;
}

int Player::Moving(void)
{
    return mMoving;
}
