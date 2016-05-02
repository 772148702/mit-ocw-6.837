#include "orthographic_camera.h"
#include <assert.h>

OrthographicCamera::OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size)
{
  m_center = center;
  m_direction = direction;
  m_direction.Normalize();
  m_up = up;
  m_up = m_up - m_direction * m_up.Dot3(m_direction);
  m_up.Normalize();
  Vec3f::Cross3(m_horizontal, m_direction, m_up);
  m_size = size;
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
  Vec3f origin =  m_center + m_horizontal * (point.x() - 0.5) * m_size + m_up * (point.y() - 0.5) * m_size;
  return Ray(m_direction, origin);
}
