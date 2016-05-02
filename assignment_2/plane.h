#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"

class Plane : public Object3D
{
 public:
  Plane(const Vec3f &normal, float d, const Vec3f &color);
  bool Intersect(const Ray &r, Hit &h, float tmin) const;
 protected:
  Vec3f m_normal;
  float m_d;
  Vec3f m_color;
};

#endif
