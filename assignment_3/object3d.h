#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "vectors.h"
#include "Ray.h"
#include "hit.h"
#include "material.h"

class Object3D
{
private:

public:
  inline virtual ~Object3D(){};
  virtual bool Intersect(const Ray &r, Hit &h, float tmin) const  = 0;
protected:
  const Material *m_material;
};

#endif
