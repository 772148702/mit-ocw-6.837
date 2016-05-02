#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"
#include "matrix.h"

class Transform : public Object3D
{
 public:
  ~Transform();
  Transform(Matrix &m, Object3D *obj);
  bool Intersect(const Ray &r, Hit &h, float tmin) const;
 private:
  Matrix m_matrix;
  Object3D* m_pObject;
};

#endif
