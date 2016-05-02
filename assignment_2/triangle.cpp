#include "triangle.h"
#include <math.h>

Triangle::Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Vec3f &color)
{
  m_a = a;
  m_b = b;
  m_c = c;
  Vec3f::Cross3(m_normal, b - a, c - a);
  m_normal.Normalize();
  m_color = color;
  Vec3f tmp;
  m_asubb = a - b;
  m_asubc = a - c;
  Vec3f::Cross3(tmp, m_asubb, m_asubc);
}

bool Triangle::Intersect(const Ray &r, Hit &h, float tmin) const
{
  bool result = false;

  const Vec3f O = r.getOrigin();
  const Vec3f V = r.getDirection();
  float NdotV = m_normal.Dot3(V);
  float A = \
    m_asubb.x() * m_asubc.y() * V.z() + \
    m_asubc.x() * V.y() * m_asubb.z() + \
    V.x() * m_asubb.y() * m_asubc.z() \
    - \
    V.x() * m_asubc.y() * m_asubb.z() - \
    m_asubc.x() * m_asubb.y() * V.z() - \
    m_asubb.x() * V.y() * m_asubc.z();

  if (NdotV != 0.0f)
    {
      Vec3f asubO = m_a - O;
      float t = \
	m_asubb.x() * m_asubc.y() * asubO.z() + \
	m_asubc.x() * asubO.y() * m_asubb.z() + \
	asubO.x() * m_asubb.y() * m_asubc.z()
	- \
	asubO.x() * m_asubc.y() * m_asubb.z() - \
	m_asubc.x() * m_asubb.y() * asubO.z() - \
	m_asubb.x() * asubO.y() * m_asubc.z();

      t /= A;

      if (t > tmin)
	{
	  float beta = \
	    asubO.x() * m_asubc.y() * V.z() + \
	    m_asubc.x() * V.y() * asubO.z() + \
	    V.x() * asubO.y() * m_asubc.z() \
	    - \
	    V.x() * m_asubc.y() * asubO.z() - \
	    m_asubc.x() * asubO.y() * V.z() - \
	    asubO.x() * V.y() * m_asubc.z();

	  beta /= A;

	  float gama = \
	    m_asubb.x() * asubO.y() * V.z() +
	    asubO.x() * V.y() * m_asubb.z() +
	    V.x() * m_asubb.y() * asubO.z() \
	    - \
	    V.x() * asubO.y() * m_asubb.z() -
	    asubO.x() * m_asubb.y() * V.z() -
	    m_asubb.x() * V.y() * asubO.z();

	  gama /= A;

	  if (beta > 0 && gama > 0 && beta + gama < 1)
	    {
	      result = true;
	      if ( t < h.getT())
		h.set(t, m_color, m_normal);
	    }
	}
    }

  return result;
}
