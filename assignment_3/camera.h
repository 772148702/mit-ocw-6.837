#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Ray.h"
#include "vectors.h"

class Camera
{
public:
  inline virtual ~Camera(){};
  virtual Ray generateRay(Vec2f point) = 0;
};

#endif
