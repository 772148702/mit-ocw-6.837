#include "sphere.h"
#include <math.h>

Sphere::Sphere(const Vec3f &center, float radius, const Vec3f &color)
{
	m_center = center;
	m_radius = radius;
	m_color  = color;
}

bool Sphere::Intersect(const Ray &r, Hit &h, float tmin) const
{
	bool result = false;

	// Ray: R(t) = O + V dot t
	// Sphere: || X - C || = r
	// Intersect equation: at^2  + bt + c = 0; a = V dot V; b = 2V dot (O - C); C = ||O - C||^2 - r^2
	// Intersect condition: b^2 - 4ac > 0
	Vec3f V = r.getDirection();
	Vec3f O = r.getOrigin();
	Vec3f tmp = O - m_center;
	float dist = tmp.Length();

	float b = 2 * V.Dot3(tmp);
	float c = dist * dist - m_radius * m_radius;
	float disc = b * b - 4 * c;


	float t = INFINITY;

	if (disc > 0)
	  {
	    float sroot = sqrt(disc);

	    float t1 = (-b - sroot) * 0.5;
	    float t2 = (-b + sroot) * 0.5;

	    if ( t1 >= tmin )
	      t = t1;
	    else if ( t2 >= tmin )
	      t = t2;

	    if ( t < h.getT() )
	      {
		h.set(t, m_color);
	      }

	    result = true;

	  }
	else
	  result = false;


	return result;
}
