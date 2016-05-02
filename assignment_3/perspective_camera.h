#ifndef _PERSPECTIVE_CAMERA_H_
#define _PERSPECTIVE_CAMERA_H_

#include "camera.h"
#include "vectors.h"

class PerspectiveCamera : public Camera
{
 public:
  PerspectiveCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float angle);
  Ray generateRay(Vec2f point);
 private:
  Vec3f m_center;
  Vec3f m_direction;
  Vec3f m_up;
  Vec3f m_horizontal;
  float m_angle;
 private:
  float m_z;
  
};

#endif


  
