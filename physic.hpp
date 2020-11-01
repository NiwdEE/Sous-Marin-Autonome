#ifdef WIN32
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


/*
	Conventions de codage :

	La première lettre du nom d'un vec3 correspond à la nature du vec3. Avec V => Vecteur, P => Point, I => Indéfini(mais l'un des deux), Rien => Autre
		P0, P1 sont des points VDir, et VVel sont des vecteurs

	La numérotation des points de fait à partir de 0

	Dans le doute, toujours normaliser les vecteurs directeurs qu'on utilise. À moins de savoir ce que l'on fait, c'est le meilleur moyen de faire de la D


*/


#ifndef DEF_Physic
#define DEF_Physic

//Objects

class Ray //Segment
{
	public:

		Ray();
		Ray(vec3 p0, vec3 p1);
		Ray(vec3 p0, vec3 dir, float len);
		~Ray();

		float getLength(void) const;
		vec3 getDirection(void) const;
		vec3 getCenter(void) const;

		vec3 getPoint(int n) const;

		void SetPoint(int n, vec3 NO);

		void Redefine();
		void Redefine(vec3 pp0, vec3 pp1);
		void Redefine(vec3 pp0, vec3 dir, float len);

	
	private:

		vec3 mP0;
		vec3 mP1;
		vec3 mPCenter;
		float mLength;
		vec3 mVDirection;

		void RefreshDatas(void);
};


namespace Plane
{

	class Infinite
	{
		public:
			Infinite();
			Infinite(vec3 p0, vec3 normal);
			Infinite(vec3 p0, vec3 dir1, vec3 dir2);
			~Infinite();

			vec3 getOrigin();
			vec3 getNormal();

		private:

			vec3 mP0;
			vec3 mVNormal;

			vec3 mVDirector0;
			vec3 mVDirector1;

			void RefreshDirectors(void);
		
	};

	class Triangle
	{
		public:
			Triangle();
			Triangle(vec3 p0, vec3 p1, vec3 p2);
			Triangle(vec3 p0, vec3 vdir0, float len0, vec3 vdir1, float len1);
			~Triangle();

			vec3 getNormal(void);
			vec3 getGravityCenter(void);
			vec3 getPoint(int n);

		private:

			vec3 mP0;
			vec3 mP1;
			vec3 mP2;

			vec3 mPGravityCenter;
			vec3 mVNormal;

			void RefreshDatas(void);
	};

	class Disc
	{
		public:
			Disc();
			Disc(vec3 origin, vec3 normal, float radius);
			Disc(vec3 origin, vec3 p1, vec3 p2);
			Disc(vec3 origin, vec3 dir1, vec3 dir2, float radius);
			~Disc();

			vec3 getCenter(void);
			vec3 getNormal(void);

			float getRadius(void);
			
		private:

			vec3 mPCenter;
			vec3 mVNormal;

			float mRadius;
	};

	
};

namespace Shape
{
	class Cylinder
	{
		public:

			Cylinder();
			Cylinder(Plane::Disc basedisc, float height);
			Cylinder(Plane::Disc basedisc, float height, bool reversenormal);
			Cylinder(vec3 BaseOrigin, vec3 BaseNormal, float radius, float height);
			~Cylinder();

		private:

			Plane::Disc mBaseDisc;
			float height;



		
	};
}


/*
namespace gte
{
    template <typename Real>
    class TIQuery<Real, Plane3<Real>, Cylinder3<Real>>
    {
    public:
        struct Result
        {
            bool intersect;
        };

        // The cylinder must have finite height.
        Result operator()(Plane3<Real> const& plane, Cylinder3<Real> const& cylinder)
        {
            LogAssert(cylinder.height != std::numeric_limits<Real>::max(),
                "Cylinder height must be finite.");

            Result result;

            // Compute extremes of signed distance Dot(N,X)-d for points on
            // the cylinder.  These are
            //   min = (Dot(N,C)-d) - r*sqrt(1-Dot(N,W)^2) - (h/2)*|Dot(N,W)|
            //   max = (Dot(N,C)-d) + r*sqrt(1-Dot(N,W)^2) + (h/2)*|Dot(N,W)|
            DCPQuery<Real, Vector3<Real>, Plane3<Real>> vpQuery;
            Real distance = vpQuery(cylinder.axis.origin, plane).distance;
            Real absNdW = std::fabs(Dot(plane.normal, cylinder.axis.direction));
            Real root = std::sqrt(std::max((Real)1 - absNdW * absNdW, (Real)0));
            Real term = cylinder.radius * root + (Real)0.5 * cylinder.height * absNdW;

            // Intersection occurs if and only if 0 is in the interval
            // [min,max].
            result.intersect = (distance <= term);
            return result;
        }
    };

    template <typename Real>
    class FIQuery<Real, Plane3<Real>, Cylinder3<Real>>
    {
    public:
        struct Result
        {
            bool intersect;

            // The type of intersection.
            //   0: none
            //   1: single line (cylinder is tangent to plane), line[0] valid
            //   2: two parallel lines (plane cuts cylinder in two lines)
            //   3: circle (cylinder axis perpendicular to plane)
            //   4: ellipse (cylinder axis neither parallel nor perpendicular)
            int type;
            Line3<Real> line[2];
            Circle3<Real> circle;
            Ellipse3<Real> ellipse;
        };

        // The cylinder must have infinite height.
        Result operator()(Plane3<Real> const& plane, Cylinder3<Real> const& cylinder)
        {
            LogAssert(cylinder.height != std::numeric_limits<Real>::max(),
                "Cylinder height must be finite.");

            Result result;

            DCPQuery<Real, Vector3<Real>, Plane3<Real>> vpQuery;
            Real sdistance = vpQuery(cylinder.axis.origin, plane).signedDistance;
            Vector3<Real> center = cylinder.axis.origin - sdistance * plane.normal;
            Real cosTheta = Dot(cylinder.axis.direction, plane.normal);
            Real absCosTheta = std::fabs(cosTheta);

            if (absCosTheta > (Real)0)
            {
                // The cylinder axis intersects the plane in a unique point.
                result.intersect = true;
                if (absCosTheta < (Real)1)
                {
                    result.type = 4;
                    result.ellipse.normal = plane.normal;
                    result.ellipse.center = cylinder.axis.origin -
                        (sdistance / cosTheta) * cylinder.axis.direction;
                    result.ellipse.axis[0] = cylinder.axis.direction -
                        cosTheta * plane.normal;
                    Normalize(result.ellipse.axis[0]);
                    result.ellipse.axis[1] = UnitCross(plane.normal,
                        result.ellipse.axis[0]);
                    result.ellipse.extent[0] = cylinder.radius / absCosTheta;
                    result.ellipse.extent[1] = cylinder.radius;
                }
                else
                {
                    result.type = 3;
                    result.circle.normal = plane.normal;
                    result.circle.center = center;
                    result.circle.radius = cylinder.radius;
                }
            }
            else
            {
                // The cylinder is parallel to the plane.
                Real distance = std::fabs(sdistance);
                if (distance < cylinder.radius)
                {
                    result.intersect = true;
                    result.type = 2;

                    Vector3<Real> offset = Cross(cylinder.axis.direction, plane.normal);
                    Real extent = std::sqrt(cylinder.radius * cylinder.radius - sdistance * sdistance);

                    result.line[0].origin = center - extent * offset;
                    result.line[0].direction = cylinder.axis.direction;
                    result.line[1].origin = center + extent * offset;
                    result.line[1].direction = cylinder.axis.direction;
                }
                else if (distance == cylinder.radius)
                {
                    result.intersect = true;
                    result.type = 1;
                    result.line[0].origin = center;
                    result.line[0].direction = cylinder.axis.direction;
                }
                else
                {
                    result.intersect = false;
                    result.type = 0;
                }
            }

            return result;
        }
    };
}*/

//Functions

bool collide(Ray ray, Plane::Infinite plane);

bool collide(Plane::Infinite plane, Ray ray);

bool collide(Ray ray, Plane::Triangle plane);

bool collide(Plane::Triangle plane, Ray ray);

bool collide(Ray ray, Plane::Disc plane);

bool collide(Plane::Disc plane, Ray ray);

#endif