#ifndef RAY_H
#define RAY_H

#include "vec3.h"

//Represents a ray in a 3D space defined by an origin point and a direction vector.
class Ray
{
    public:
        Point3 orig;    //origin of the light ray
        Vec3 dir;       //direction vector the ray is traveling

    public:
        //Constructors
        Ray() {}
        Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction)
        {}

        //Getter member functions
        Point3 origin() const {return orig;}
        Vec3 direction() const {return dir;}

        //Returns the position of the light ray at a given distance t along its direction
        Point3 at(double t) const
        {
            return orig + (t * dir);
        }
};

#endif