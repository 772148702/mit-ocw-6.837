#include "plane.h"

Plane::Plane(const Vec3f &normal, float d, const Material *material)
{
  m_normal = normal;
  m_d = d;
  m_material = material;

  m_bounding_box_max.Set(INFINITY, INFINITY, INFINITY);
  m_bounding_box_max.Set(-INFINITY, -INFINITY, -INFINITY);
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
			h.set(t, m_material, m_normal, r);
	  }
    }

  return result;
}
