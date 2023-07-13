#ifndef __SPHERE_H_
#define __SPHERE_H_

#include <cmath>
#include <iostream>
#include "rt.h"
#include "vec.h"

namespace kart
{
  class sphere : public shape
  {
  private:
    vec center;
    double rad;
  public:
    sphere(const vec& c, const double& r,const material& h) : center(c), rad(r) , shape(h) {}
    double intersect(const ray& r) override
    {
      double discriminant = ((r.dir & (r.org - center)) * (r.dir & (r.org - center)) - ((r.org - center) & (r.org - center)) + rad * rad );
      if (discriminant < -0.00001)
        return 0;
      double t1 = -(r.dir & (r.org - center)) - discriminant;
      double t2 = -(r.dir & (r.org - center)) + discriminant;
      if (t1 > 0.00001)
        return t1;
      else if (t1 > 0.00001)
        return t2;
      else
        return 0;
    }
    vec normal(const vec& p) override
    {
      return (p - center).Normalize();
    }
  };
}

#endif