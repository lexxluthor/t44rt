#ifndef __RT_H_
#define __RT_H_

//#include <glut.h>
#include <thread>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <utility>
#include "vec.h"
#include "ray.h" 
#include "frame.h"

namespace kart
{
  class material
  {
  public:
      const char* name = "";
      vec amb{}, dif{}, spec{};
      dbl ph, refr;

      material()
      {
          ph = refr = 1;
      }

      material(const char* n_name,
          const vec& n_amb, const vec& n_dif, const vec& n_spec,
          const dbl& n_ph, const dbl& n_refr = 1) : name(n_name), amb(n_amb), dif(n_dif), spec(n_spec), ph(n_ph), refr(n_refr)
      {
      }
  };

  class shape
  {
  public:
    explicit shape(material new_mtl) : mtl(new_mtl) {}
    virtual double intersect(const ray&)
    {
      return 0;
    }
    virtual vec normal(const vec& p)
    {
      return {0, 0, 0};
    }
    material mtl;
  };


  class light_source
  {
  public:
      virtual vec dest(const vec& point)
      {
          return {0, 0, 0};
      }
      vec color;
  };

  class directed_source : public light_source
  {
  private:
      vec direction;
  public:
      directed_source(vec p, vec h) : direction(p.Normalize())
      {
          color = h;
      }
      vec dest(const vec& point) override
      {
          return direction;
      }
  };

  class point_source : public light_source
  {
  private:
      vec position;
  public:
      point_source(vec p, vec h) : position(p)
      {
          color = h;
      }
      vec dest(const vec& point) override
      {
          return (position - point).Normalize();
      }
  };


  class rt
  {
  public:
    std::vector<shape*> scene;
    std::vector<light_source*> lights;
    rt& operator<<(shape* object)
    {
      scene.push_back(object);
      return *this;
    }
    rt& operator<<(light_source* source)
    {
      lights.push_back(source);
      return *this;
    }
    frame shot;

    rt() : shot(1920, 1080){}
    ~rt()
    {
      for (auto x : scene)
        delete x;
      scene.clear();
    }
    static double clamp(double a, double b, double x)
    {
      if (x < a)
        return a;
      if (x > b)
        return b;
      return x;
    }
    static vec check_norm(const vec& dir, shape* object, const vec& point)
    {
      vec normal = object->normal(point);
      if ((dir & normal) > 0)
        normal = normal*(-1);
      return normal;
    }
    std::pair<shape*, vec> intersect( const ray& r )
    {
      double intersection = 0;
      shape* closest = nullptr;
      vec point = vec(0, 0, 0);

      for (auto object : scene)
      {
        double t = object->intersect(r);
        if (t > 0 && (closest == nullptr || t < intersection))
          intersection = t, closest = object;  
      }

      if (closest != nullptr)
        point = r(intersection);

      return {closest, point};
    }
    vec trace(const ray& r, const int depth)
    {
      if (depth >= 8)
        return {0, 0, 0};
      std::pair<shape*, vec> intersection = intersect(r);
      if (intersection.first != nullptr)
        return shade(r, intersection.first, intersection.second, depth);
      return {0, 0, 0};
    }

    vec shade( const ray& r, shape* object, const vec& point, int depth)
    {
      const vec dir = r.dir;
      const vec nl = check_norm(dir, object, point);
      const vec refl = (dir - nl * 2 * (nl & dir)).Normalize();
      vec amb = object->mtl.amb;
      vec reflect = (vec(0.2, 0.2, 0.2) | trace(ray(point + nl * 0.001, refl), depth + 1));

      vec dif = vec(0, 0, 0), specular = vec(0, 0, 0);
      for (auto light : lights)
      {
          const vec d = light->dest(point);

          if (intersect(ray(point + d * 0.0000001, d)).first == nullptr)
          {
              if ((nl & d) > 0)
                  dif += (object->mtl.dif | light->color) * (nl & d);
              if ((refl & d) > 0)
                  specular += (object->mtl.spec | light->color) * pow(refl & d, object->mtl.ph);
          }
      }
      return amb + dif + specular + reflect;
    }
    void Render()
    {
      
      const vec from(10, 10, 10);
      const vec to(0, 0, 0);
      vec up = vec(0, 1, 0).Normalize();
      const vec direction = (to - from).Normalize();
      const vec right = (direction % up).Normalize();
      up = (right % direction).Normalize();

      const double size = 0.1,
          wp = shot.w > shot.h ? size * shot.w / shot.h : size,
          hp = shot.w > shot.h ? size : size * shot.h / shot.w,
          proj_dist = 2 * size,
          ws = shot.w,
          hs = shot.h;

      auto calc = [&](const int& ys, const int yf) {
          for (int y = ys; y < yf; y++)
              for (int x = 0; x < shot.w; x++) {
                  const vec p = direction * proj_dist +
                      right * ((x - ws / 2 + 0.5) * wp / ws) +
                      up * ((hs / 2 - y - 0.5) * hp / hs);

                  const ray r(p + from, p);
                  shot.put_pixel(x, y, trace(r, 0));
              }
      };

      std::vector<std::thread> tasks;
      int n = std::thread::hardware_concurrency();
      int delta = shot.h / n;
      int ycur = 0;
      for (int i = 0; i < n; i++)
      {
          if (i == n - 1)
              tasks.emplace_back(std::thread([=] { calc(ycur, shot.h); }));
          else
              tasks.emplace_back(std::thread([=] { calc(ycur, ycur + delta); }));
          ycur += delta;
      }

      for (auto& tsk : tasks)
          tsk.join();
    }
  };

  extern rt MainScene;
}

#endif

