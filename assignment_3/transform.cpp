#include "transform.h"
#include <assert.h>

Transform::~Transform()
{
}

Transform::Transform(Matrix &m, Object3D *obj)
{
  m_matrix = m;
  m_pObject = obj;
}

bool Transform::Intersect(const Ray &r, Hit &h, float tmin) const
{
  bool result = false;
  
  Matrix m = m_matrix;
  if ( m.Inverse() )
    {
      Vec3f org = r.getOrigin();
      Vec3f dir = r.getDirection();
      m.Transform(org);
      m.TransformDirection(dir);
      Ray r2 (dir, org);
      result = m_pObject->Intersect(r2, h, tmin);
      
      if (result)
	{
	  Matrix m1 = m;
	  m1.Transpose();
	  Vec3f n = h.getNormal();
	  m1.TransformDirection(n);
	  n.Normalize();
	  h.set(h.getT(), h.getMaterial(), n, r);
	}
    }
  return result;
}
