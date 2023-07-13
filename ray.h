#ifndef _ray_h_
#define _ray_h_

#include "vec.h"
class ray
{
public:
    vec org, dir;

    ray(const vec& o, const vec& x) : org(o), dir(x)
    {
        dir = dir.Normalize();
    }
    ~ray() =default;

    vec operator()(const dbl& T) const
    {
        return org + dir * T;
    }
};

#endif //_ray_h_