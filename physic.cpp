#include "physic.hpp"

//=================================================================     SEGMENT     =================================================================

Ray::Ray(): mP0(vec3(0,0,0)), mP1(vec3(0,0,0)), mPCenter(vec3(0,0,0)), mLength(-1), mVDirection(vec3(0,0,0))
{

}

Ray::Ray(vec3 pp0, vec3 pp1): mP0(pp0), mP1(pp1), mPCenter(vec3(0,0,0)), mLength(-1), mVDirection(vec3(0,0,0))
{
	this->RefreshDatas();
}

Ray::Ray(vec3 pp0, vec3 dir, float len): mP0(pp0), mLength(len), mVDirection(dir)
{
	mP1 = pp0 + len*normalize(dir);

	mPCenter = 0.5f*(pp0 + mP1);
}

Ray::~Ray()
{

}

void Ray::RefreshDatas(void)
{
	mLength = distance(mP0, mP1);

	mVDirection = normalize(mP1-mP0);

	mPCenter = 0.5f*(mP0 + mP1);
}

float Ray::getLength(void) const
{
	return mLength;
}


vec3 Ray::getDirection(void) const
{
	return mVDirection;
}


vec3 Ray::getCenter(void) const
{
	return mPCenter;
}


vec3 Ray::getPoint(int n) const
{
	if(n==0)
		return mP0;
	else
		return mP1;
}

void Ray::SetPoint(int n, vec3 NO)
{
	if(n==0)
		mP0 = NO;
	else
		mP1 = NO;

	this -> RefreshDatas();
}

void Ray::Redefine()
{
	mP0 = vec3(0,0,0);
	mP1 = vec3(0,0,0);
	mPCenter = vec3(0,0,0);
	mLength = -1;
	mVDirection = vec3(0,0,0);
}

void Ray::Redefine(vec3 pp0, vec3 pp1)
{
	mP0 = pp0;
	mP1 = pp1;
	this->RefreshDatas();
}

void Ray::Redefine(vec3 pp0, vec3 dir, float len)
{
	mP0 = pp0;
	mVDirection = dir;
	mLength = len;
	mP1 = pp0 + len*normalize(dir);

	mPCenter = 0.5f*(pp0 + mP1);
}

// ===============================================================    Fin SEGMENT     ===============================================================



//==================================================================     PLAN      ==================================================================

	//=============================================================     INFINI     =============================================================	

Plane::Infinite::Infinite(): mP0(vec3(0,0,0)), mVNormal(vec3(0,0,0)), mVDirector0(vec3(0,0,0)), mVDirector1(vec3(0,0,0))
{

}

Plane::Infinite::Infinite(vec3 p0, vec3 normal): mP0(p0), mVNormal(normal), mVDirector0(vec3(0,0,0)), mVDirector1(vec3(0,0,0))
{

}

Plane::Infinite::Infinite(vec3 p0, vec3 dir1, vec3 dir2): mP0(p0), mVNormal(normalize(cross(dir1, dir2))), mVDirector0(dir1), mVDirector1(dir2)
{

}

Plane::Infinite::~Infinite()
{

}

void Plane::Infinite::RefreshDirectors(void)
{

	if(mVNormal.z != 0)
		mVDirector0 = normalize( vec3(1, 1, -(mVNormal.x+mVNormal.y)/mVNormal.z) );
	else if(mVNormal.y != 0)
		mVDirector0 = normalize( vec3(1, -(mVNormal.x+mVNormal.z)/mVNormal.y, 1) );
	else if(mVNormal.x != 0)
		mVDirector0 = normalize( vec3(-(mVNormal.y+mVNormal.z)/mVNormal.x, 1, 1) );
	else 
		return;

	mVDirector1 = normalize(cross(mVDirector0, mVNormal));
}


vec3 Plane::Infinite::getOrigin()
{
	return mP0;
}

vec3 Plane::Infinite::getNormal()
{
	return mVNormal;
}

	//===========================================================     Fin INFINI     ===========================================================	

	//============================================================     TRIANGLE     ============================================================	
	
Plane::Triangle::Triangle(): mP0(vec3(0,0,0)), mP1(vec3(0,0,0)), mP2(vec3(0,0,0)), mPGravityCenter(vec3(0,0,0)), mVNormal(vec3(0,0,0))
{

}

Plane::Triangle::Triangle(vec3 p0, vec3 p1, vec3 p2): mP0(p0), mP1(p1), mP2(p2), mPGravityCenter(vec3(0,0,0)), mVNormal(vec3(0,0,0))
{
	this->RefreshDatas();
}

Plane::Triangle::Triangle(vec3 p0, vec3 vdir1, float len1, vec3 vdir2, float len2): mP0(p0), mP1(p0 + len1*vdir1), mP2(p0 + len2*vdir2), mPGravityCenter(vec3(0,0,0)), mVNormal(vec3(0,0,0))
{
	this->RefreshDatas();
}

Plane::Triangle::~Triangle()
{

}

void Plane::Triangle::RefreshDatas(void)
{
	mPGravityCenter = float(1.0/3)*(mP0 + mP1 + mP2);

	mVNormal = normalize(cross(mP1 - mP0, mP2 - mP0));
}

vec3 Plane::Triangle::getNormal(void)
{
	return mVNormal;
}

vec3 Plane::Triangle::getGravityCenter(void)
{
	return mPGravityCenter;
}

vec3 Plane::Triangle::getPoint(int n)
{
	if(n<1)
		return mP0;
	else if(n>1)
		return mP2;
	else
		return mP1;
}

	//==========================================================     Fin TRIANGLE     ==========================================================	
	
	//=============================================================     DISQUE     =============================================================


Plane::Disc::Disc(): mPCenter(vec3(0,0,0)), mVNormal(vec3(0,0,0)), mRadius(0)
{

}

Plane::Disc::Disc(vec3 origin, vec3 normal, float radius): mPCenter(origin), mVNormal(normal), mRadius(radius)//Constructeur conseillé
{

}

Plane::Disc::Disc(vec3 origin, vec3 p1, vec3 p2): mPCenter(origin), mVNormal(normalize(cross(p1 - origin, p2 - origin))), mRadius(length(p1 - origin))//Constructeur déconseillé
{

}

Plane::Disc::Disc(vec3 origin, vec3 dir1, vec3 dir2, float radius): mPCenter(origin), mVNormal(normalize(cross(dir1, dir2))), mRadius(radius)//Constructeur conseillé
{

}

Plane::Disc::~Disc()
{

}

vec3 Plane::Disc::getCenter(void)
{
	return mPCenter;
}

vec3 Plane::Disc::getNormal(void)
{
	return mVNormal;
}

float Plane::Disc::getRadius(void)
{
	return mRadius;
}
	
	//===========================================================     Fin DISQUE     ===========================================================

// ================================================================     Fin PLAN     ================================================================

/*struct Sphere
{
    vec3 O;
    uint ray;
};*/

/*
//Version Bolosse
bool Point3DIn2DTriangle(vec3 m, vec3 p1, vec3 p2, vec3 p3){

    if(m == p1 || m == p2 || m == p3)
        return true;

    vec3 P1P2 = p2 - p1;
    vec3 P1P3 = p3 - p1;
    vec3 P1M  = m  - p1;

    float ColFact_P1M_P1P2 = (dot(P1M, P1P2)/dot(P1P2, P1P2));  //Facteur de colinéarité entre la projection de P1M sur P1P2 et P1P2
    vec3 proj_P1M_P1P2 = ColFact_P1M_P1P2 * P1P2;               //Vecteur correspondant à la projection de P1M sur P1P2
    float ColFact_P1M_P1P3 = (dot(P1M, P1P3)/dot(P1P3, P1P3));  //Facteur de colinéarité entre la projection de P1M sur P1P3 et P1P3
    vec3 proj_P1M_P1P3 = ColFact_P1M_P1P3 * P1P3;               //Vecteur correspondant à la projection de P1M sur P1P3

    if(length(proj_P1M_P1P2) < length(P1P2)
    && ColFact_P1M_P1P2 > 0
    && length(proj_P1M_P1P3) < length(P1P3)
    && ColFact_P1M_P1P3 > 0)
        return true;
    

    return false;

}*/

//Version mâle Alpha (incompréhensible (et pas (en tout cas presque pas) commentée) mais plus Optimisée)
bool Point3DIn2DTriangle(vec3 m, vec3 p1, vec3 p2, vec3 p3){

    if(m == p2 || m == p3)
        return true;

    vec3 P1P2 = p2 - p1;
    vec3 P1P3 = p3 - p1;
    vec3 P1M;
    if( (P1M = m  - p1) == vec3(0,0,0))
    	return true;

    float ColFact_P1M_P1P2;
    float ColFact_P1M_P1P3;

    if((ColFact_P1M_P1P2 = (dot(P1M, P1P2)/dot(P1P2, P1P2))) < 0)
    	return false;
    if(length(ColFact_P1M_P1P2 * P1P2) > length(P1P2))
    	return false;
    if((ColFact_P1M_P1P3 = (dot(P1M, P1P3)/dot(P1P3, P1P3))) < 0)
    	return false;
    if(length(ColFact_P1M_P1P3 * P1P3) > length(P1P3))
    	return false;

    return true;

}

float DistancePointPlane(vec3 M, vec3 PlanePt, vec3 PlaneNormal){
	float PlaneEquationFactorD = dot(PlanePt, PlaneNormal);
	return abs( PlaneNormal.x*M.x + PlaneNormal.y*M.y + PlaneNormal.z*M.z + PlaneEquationFactorD ) / length(PlaneNormal);
}

bool collide(Ray ray, Plane::Infinite plane){

	// if(DistancePointPlane(ray.getPoint(0), plane.getOrigin(), plane.getNormal()) > ray.getLength() )
	// 	return false;

    float denom = dot(plane.getNormal(), ray.getDirection());

    if(denom > 1e-6 || denom < 1e-6){
        float t = dot(plane.getOrigin()-ray.getPoint(0), plane.getNormal())/denom;
        if (t>=0 && t<ray.getLength())
            return true;
    }

    return false;
}

bool collide(Plane::Infinite plane, Ray ray){
    return collide(ray, plane);
}

bool collide(Ray ray, Plane::Triangle plane){

    float denom = dot(plane.getNormal(), ray.getDirection());

    if(denom > 1e-6 || denom < 1e-6){
        float t = dot(plane.getGravityCenter()-ray.getPoint(0), plane.getNormal())/denom;
        if (t>=0 && t<ray.getLength() && Point3DIn2DTriangle(ray.getPoint(0) + t*ray.getDirection(), plane.getPoint(0), plane.getPoint(1), plane.getPoint(2)))
            return true;
    }

    return false;
}

bool collide(Plane::Triangle plane, Ray ray){
    return collide(ray, plane);
}

bool collide(Ray ray, Plane::Disc plane){

    float denom = dot(plane.getNormal(), ray.getDirection());

    if(denom > 1e-6 || denom < 1e-6){
        float t = dot(plane.getCenter()-ray.getPoint(0), plane.getNormal())/denom;
        if (t>=0 && t<ray.getLength() && length(plane.getCenter()-(ray.getPoint(0) + t*ray.getDirection())) < plane.getRadius() )
            return true;
    }

    return false;
}

bool collide(Plane::Disc plane, Ray ray){
    return collide(ray, plane);
}