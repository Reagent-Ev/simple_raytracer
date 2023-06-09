#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

//Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Mathematical constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265335897932385;

//Utility Functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double random_double()
{
    //Returns a random real in [0, 1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    //Return a random realn in [min, max).
    return min + (max - min) * random_double();
}

//clamp a value x between a range of [min, max]
inline double clamp(double x, double min, double max)
{
    if(x < min)
    {
        return min;
    }

    if(x > max)
    {
        return max;
    }

    return x;
}

//Common Headers
#include "ray.h"
#include "vec3.h"

#endif