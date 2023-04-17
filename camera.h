#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"

//Camera class for representing a virtual camera in 3D space and related scene scampling.

class Camera
{
    private:
        Point3 origin;                  //Origin of the camera
        Point3 lower_left_corner;       //lower left corner of the image plane
        Vec3 horizontal;                //the horizontal vector of the image plane
        Vec3 vertical;                  //the vertical vector of the image plane
        Vec3 u, v, w;
        double lens_radius;

    public:
        //Constructor to initialize camera properties
        Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist)
        {
            //Compute the orthonormal basis for the cameras coordinate system
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

            lens_radius = aperture / 2;
        }

        //Compute a ray from the camera through a given (u, v) coordinate in the image plane
        Ray get_ray(double s, double t) const
        {
            Vec3 rd = lens_radius * random_in_unit_disk();
            Vec3 offset = u * rd.x() + v * rd.y();

            return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
        }
};

#endif