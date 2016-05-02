#include "sphere.h"
#include <math.h>

Sphere::Sphere(const Vec3f &center, float radius, const Material *material)
{
	m_center = center;
	m_radius = radius;
	m_material  = material;
}

bool Sphere::Intersect(const Ray &r, Hit &h, float tmin) const
{
	bool result = false;

	// Ray: R(t) = O + V * t
	// Sphere: || X - C || = r
	// Intersect equation: at^2  + bt + c = 0; a = V dot V; b = 2V dot (O - C); C = ||O - C||^2 - r^2
	// Intersect condition: b^2 - 4ac > 0
	const Vec3f V = r.getDirection();
	const Vec3f O = r.getOrigin();
	Vec3f tmp = O - m_center;
	float dist = tmp.Length();

	float a = V.Dot3(V);
	float b = 2.0f * V.Dot3(tmp);
	float c = dist * dist - m_radius * m_radius;
	float disc = b * b - 4.0f * a * c;


	float t = INFINITY;

	if (disc > 0)
	  {
	    float sroot = sqrt(disc);
	    float tmp2 = 1.0f / (2.0f * a);

	    float t1 = (-b - sroot) * tmp2;
	    float t2 = (-b + sroot) * tmp2;
		  
		assert(t1 <= t2);

	    if ( t1 > tmin ) {
	      t = t1;
	      result = true;
	    } else if ( t2 > tmin ) {
	      t = t2;
	      result = true;
		}

	    if ( t < h.getT() )
	      {
		Vec3f n = tmp + V * t;
		n.Normalize();
		h.set(t, m_material, n, r);
	      }

	  }



	return result;
}
