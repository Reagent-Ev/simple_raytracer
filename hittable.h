#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "math_utils.h"

class Material;

//structure containing information about the ray-object intersection
struct hit_record
{
    Point3 p;                       //Point of intersection between the ray and the object
    Vec3 normal;                    //The surface normal at the point of intersection
    shared_ptr<Material> mat_ptr;   //Shared pointer object to Material objects
    double t;                       //Distance along the ray to the point of intersection
    bool front_face;                //Whether the ray hit the front face of the object or not.

    //Determine wheter the ray hit the front face of object or back face setting the surface normal accordingly
    inline void set_face_normal(const Ray& r, const Vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

//Abstract class representing any object that is hittable(can be interacted with) by a ray
//inherited classes must implement the hit method.
class Hittable
{
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif