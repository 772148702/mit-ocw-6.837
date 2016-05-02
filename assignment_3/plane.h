#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"

class Plane : public Object3D
{
 public:
  Plane(const Vec3f &normal, float d, const Material *material);
  bool Intersect(const Ray &r, Hit &h, float tmin) const;
 private:
  Vec3f m_normal;
  float m_d;
};

#endif
