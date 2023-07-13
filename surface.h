#ifndef __PLANE_H_
#define __PLANE_H_

#include <cmath>
#include <iostream>
#include "rt.h"
#include "vec.h"

namespace kart
{
  class surface : public shape
  {
  private:
    vec point, norm;
  public:
    surface(const vec& p, const vec& n, const material& h) : point(p), norm(n.Normalize()), shape(h) {}
    double intersect(const ray& r) override
    {
      static const double np = norm & point;
      const double nd = (norm & r.dir);
      if (fabs(nd) < 0.0001) 
        return -1;
      return (np - (r.org & norm)) / nd;
    }
    vec normal(const vec& p) override
    {
      return norm;
    }
  };
}

#endif