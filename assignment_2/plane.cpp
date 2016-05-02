#include "plane.h"
#include <math.h>

Plane::Plane(const Vec3f &normal, float d, const Vec3f &color)
{
  m_normal = normal;
  m_d = d;
  m_color = color;
  
}

bool Plane::Intersect(const Ray &r, Hit &h, float tmin) const
{
  bool result= false;

  const Vec3f O = r.getOrigin();
  const Vec3f V = r.getDirection();
  float NdotV = m_normal.Dot3(V);

  if (NdotV != 0.0f)
    {
      float t = - (m_normal.Dot3(O) - m_d) / NdotV;
      if (t > tmin)
	{
	  result = true;
	  if ( t < h.getT())
	    h.set(t, m_color, m_normal);
	}
    }

  return result;
}
