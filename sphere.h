#ifndef SPHERE_H
#define SPHERE_H


#include "hittable.h"
#include "vec3.h"

//Class representing a hittable sphere object
class Sphere : public Hittable
{
    public:
        Point3 center;              //Point of the center of the sphere
        double radius;              //Radius of the sphere
        shared_ptr<Material> mat_ptr;   //Pointer to the material the sphere is made out of

    public:
        //Constructors
        Sphere() {}
        Sphere(Point3 cen, double r, shared_ptr<Material> m) : center(cen), radius(r), mat_ptr(m)
        {};

        //Overriden virtual function to calculate the where the ray hit the sphere object
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
};

//Compute the intersection between a ray and a sphere if hit returns true and fills in
//the information about the ray intersection int the hit record.
bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    Vec3 oc = r.origin() - center;                  //compute the vector from the ray origin to the center of the sphere
    auto a = r.direction().length_squared();        //Find the coefficients of the quadratic equation for the intersection
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    //Compute the discriminant of the quadratic equation, if its negative the ray missed the sphere
    auto discriminant = half_b * half_b - (a * c);
    if(discriminant < 0)
        return false;
    
    auto sqrtdis = sqrt(discriminant);

    //Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtdis) / a;
    if(root < t_min || t_max < root)
    {
        root = (-half_b + sqrtdis) / a;
        if(root < t_min || t_max < root)
        {
            return false;
        }
    }

    //Fill in the hit record with all the information about the ray intersection
    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;        //The ray hit the sphere
}

#endif