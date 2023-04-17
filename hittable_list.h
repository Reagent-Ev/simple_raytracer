#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class Hittable_list : public Hittable
{
    public:
        //c++ std::vector of objects pointers to hittable objects
        std::vector<shared_ptr<Hittable>> objects;

    public:
        //Constructors
        Hittable_list() {}
        Hittable_list(shared_ptr<Hittable> object) {add(object); }

        //clear the vector of objects and add new hittable objects.
        void clear() {objects.clear(); }
        void add(shared_ptr<Hittable> object) { objects.push_back(object); }

        //Compute the intersection between a ray and the list of hittable objects
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
};


bool Hittable_list::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    //Loop over all hittable objects in the list and find the closest hit
    for(const auto& object : objects)
    {
        if(object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif