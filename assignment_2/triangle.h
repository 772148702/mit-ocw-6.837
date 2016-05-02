#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"

class Triangle : public Object3D
{
 public:
  Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Vec3f &color);
  bool Intersect(const Ray &r, Hit &h, float tmin) const;
 protected:
  Vec3f m_a;
  Vec3f m_b;
  Vec3f m_c;
  Vec3f m_normal;

  Vec3f m_color;
 private:
  Vec3f m_asubb;
  Vec3f m_asubc;
};

#endif
