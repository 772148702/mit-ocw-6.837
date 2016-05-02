#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "vectors.h"
#include "Ray.h"
#include "hit.h"

class Object3D
{
public:
  inline virtual ~Object3D(){};
  virtual bool Intersect(const Ray &r, Hit &h, float tmin) const  = 0;
protected:
  Vec3f m_color;
};

#endif
