#include "perspective_camera.h"
#include <assert.h>

PerspectiveCamera::PerspectiveCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float angle)
{
  m_center = center;
  m_direction = direction;
  m_direction.Normalize();
  m_up = up;
  m_up = m_up - m_direction * m_up.Dot3(m_direction);
  m_up.Normalize();
  Vec3f::Cross3(m_horizontal, m_direction, m_up);
  m_angle = angle;
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
  float size = tan(m_angle / 2.0f) * 2.0f;
  Vec3f direction =  m_horizontal * (point.x() - 0.5) * size + m_up * ( point.y() - 0.5) * size + m_direction;
  return Ray(direction, m_center);
}

