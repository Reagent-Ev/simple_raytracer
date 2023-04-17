#ifndef MATERIAL_H
#define MATERIAL_H

#include "math_utils.h"
#include "hittable.h"

struct hit_record;

//Abstract material base class represents the type of material the world objects are made of,
//scatter function calculates how light should scatter when hit the object.
class Material
{
    public:
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const = 0;
};

//Lambertian material that diffusely reflects incoming light in all directions for matte surfaces.
class Lambertian: public Material
{
    public:
        Color albedo;   //Diffuse reflection coefficient of the material

    public:
        //Construcotr setting the color albedo
        Lambertian(const Color& a) : albedo(a)
        {}

        //Scatter the incoming light ray by randomly selecting a direction in a hemisphere around the hit point
        //and attenuating it by the diffuse reflection coefficient.
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
        {
            auto scatter_direction = rec.normal + random_unit_vector();

            //Catch the degenerate scatter direction
            if(scatter_direction.near_zero())
            {
                scatter_direction = rec.normal;
            }
            
            scattered = Ray(rec.p, scatter_direction);
            attenuation = albedo;

            return true;
        }
};

//Metal material class for metalic surfaced objects
class Metal: public Material
{
    public:
        Color albedo;
        double fuzz;    //Adds a fuzz, a reflectivity parameter that controls the amount of roughness in the reflection

    public:
        Metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1)
        {}

        //Scatter reflects incoming rays specurlarly like a polished metal surface
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
        {
            Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;

            return (dot(scattered.direction(), rec.normal) > 0);
        }
};

//Material for glass and transparent objects that refracts and reflects incoming light rays based on
//the refrative index of the medium.
class Dielectric: public Material
{
    public:
        double ir; //Index of Refraction

    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction)
        {}

        //Scatter an incoming ray by randomly choosing between reflection and refraction based on angle of incidence.
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
        {
            attenuation = Color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ ir) : ir;

            Vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
            Vec3 direction;

            //Choose whether to reflect the ray or refract it
            if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            {
                direction = reflect(unit_direction, rec.normal);
            }
            else
            {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = Ray(rec.p, direction);
            return true;
        }

        private:
            static double reflectance(double cosine, double ref_idx)
            {
                //Use Schlick's approximation for reflectance.
                auto r0 = (1 - ref_idx) / (1 + ref_idx);
                r0 = r0 * r0;

                return r0 + (1 - r0) * pow((1 - cosine), 5);
            }
};

#endif