#include "math_utils.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include<iostream>

//Check if a ray intersects with a sphere at a given center and radius
double hit_sphere(const Point3& center, double radius, const Ray& r)
{
    //Calculate the vector from the center of the sphere to the origin of the ray
    Vec3 oc = r.origin() - center;

    //Calculate the quadratic coefficients for the intersection formula
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    //Calculate the discriminant of the quadratic equation
    auto discriminant = half_b * half_b - (a * c);

    //If discriminant is negative, there is no intersection
    if(discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

//Calculate the color of the ray by its intersection with various objects
Color ray_color(const Ray& r, const Hittable_list& world, int depth)
{
    hit_record rec;

    //If exceeded the ray bounce limit, no more light is gathered, set pixel to black.
    if(depth <= 0)
    {
        return Color(0, 0, 0);
    }

    //Check for intersections with the objects in the scene
    if(world.hit(r, 0.001, infinity, rec))
    {
        Ray scattered;
        Color attenuation;

        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            //If the object is reflective or refractive, continue tracing rays recursively. For reflections.
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return Color(0, 0, 0);
    }

    //If the ray doesnt hit any objects draw a background gradient instead.
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

//The final scene image including all our objects and their materials.
Hittable_list final_scene()
{
    Hittable_list world;

    //create the ground material as a matte surface.
    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    //Populate the world with various random objects of various materials
    for(int a = -11; a < 11; a++)
    {
        for(int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphere_material;

                if(choose_mat < 0.8)
                {
                    //diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if(choose_mat < 0.5)
                {
                    //metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    //glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    //Add three final large spheres of each material, Lambertian(matte), Metal(metallic), Dielectric(glass)
    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{
    //Image dimensions, specifications
    const auto aspect_ratio = 16.0 / 9.0; //3.0 / 2.0;
    const int image_width = 900; //1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 375; //500;
    const int max_depth = 50;

    //World to render
    auto world = final_scene();

    //Camera
    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    //Rendering the image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for(int i = 0; i < image_width; i++)
        {
            Color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; s++)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            //write the individual pixel colors to standard output.
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}