#include "triangle.h"
#include <math.h>
#include <GL/freeglut.h>

Triangle::Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Material *material)
{
	float x_min, y_min, z_min;
	float x_max, y_max, z_max;

	m_a = a;
	m_b = b;
	m_c = c;

	x_min = (a.x() < b.x()) ? a.x() : b.x();
	x_min = (x_min < c.x()) ? x_min : c.x();
	y_min = (a.y() < b.y()) ? a.y() : b.y();
	y_min = (y_min < c.y()) ? y_min : c.y();
	z_min = (a.z() < b.z()) ? a.z() : b.z();
	z_min = (z_min < c.z()) ? z_min : c.z();

	x_max = (a.x() > b.x()) ? a.x() : b.x();
	x_max = (x_max > c.x()) ? x_max : c.x();
	y_max = (a.y() > b.y()) ? a.y() : b.y();
	y_max = (y_max > c.y()) ? y_max : c.y();
	z_max = (a.z() > b.z()) ? a.z() : b.z();
	z_max = (z_max > c.z()) ? z_max : c.z();

	m_bounding_box_min.Set(x_min, y_min, z_min);
	m_bounding_box_max.Set(x_max, y_max, z_max);

	Vec3f::Cross3(m_normal, b - a, c - a);
	m_normal.Normalize();
	m_material = material;
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
		  h.set(t, m_material, m_normal, r);
	    }
	}
    }

  return result;
}

void Triangle::paint() const
{
	Object3D::paint();

	glNormal3f(m_normal.x(), m_normal.y(), m_normal.z());

	glBegin(GL_TRIANGLES);
	glVertex3f(m_a.x(), m_a.y(), m_a.z());
	glVertex3f(m_b.x(), m_b.y(), m_b.z());
	glVertex3f(m_c.x(), m_c.y(), m_c.z());
	glEnd();
}