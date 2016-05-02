#include "transform.h"
#include <assert.h>

Transform::~Transform()
{
}

Transform::Transform(Matrix &m, Object3D *obj)
{
  m_matrix = m;
  m_pObject = obj;

  Vec3f _v1 = obj->GetBoundingBoxMax();
  Vec3f _v2 = obj->GetBoundingBoxMin();
  float _x1 = _v1.x(), _y1 = _v1.y(), _z1 = _v1.z();
  float _x2 = _v2.x(), _y2 = _v2.y(), _z2 = _v2.z();
  Vec3f _v3(_x2, _y1, _z1);
  Vec3f _v4(_x2, _y2, _z1);
  Vec3f _v5(_x1, _y2, _z1);
  Vec3f _v6(_x1, _y2, _z2);
  Vec3f _v7(_x1, _y1, _z2);
  Vec3f _v8(_x2, _y1, _z2);

  m_matrix.Transform(_v1);
  m_matrix.Transform(_v2);
  m_matrix.Transform(_v3);
  m_matrix.Transform(_v4);
  m_matrix.Transform(_v5);
  m_matrix.Transform(_v6);
  m_matrix.Transform(_v7);
  m_matrix.Transform(_v8);

  _x1 = _v1.x(), _y1 = _v1.y(), _z1 = _v1.z();
  _x2 = _v2.x(), _y2 = _v2.y(), _z2 = _v2.z();
  float _x3 = _v3.x(), _y3 = _v3.y(), _z3 = _v3.z();
  float _x4 = _v4.x(), _y4 = _v4.y(), _z4 = _v4.z();
  float _x5 = _v5.x(), _y5 = _v5.y(), _z5 = _v5.z();
  float _x6 = _v6.x(), _y6 = _v6.y(), _z6 = _v6.z();
  float _x7 = _v7.x(), _y7 = _v7.y(), _z7 = _v7.z();
  float _x8 = _v8.x(), _y8 = _v8.y(), _z8 = _v8.z();

  float _xmin = _x1 < _x2 ? _x1 : _x2;
  _xmin = _xmin < _x3 ? _xmin : _x3;
  _xmin = _xmin < _x4 ? _xmin : _x4;
  _xmin = _xmin < _x5 ? _xmin : _x5;
  _xmin = _xmin < _x6 ? _xmin : _x6;
  _xmin = _xmin < _x7 ? _xmin : _x7;
  _xmin = _xmin < _x8 ? _xmin : _x8;

  float _ymin = _y1 < _y2 ? _y1 : _y2;
  _ymin = _ymin < _y3 ? _ymin : _y3;
  _ymin = _ymin < _y4 ? _ymin : _y4;
  _ymin = _ymin < _y5 ? _ymin : _y5;
  _ymin = _ymin < _y6 ? _ymin : _y6;
  _ymin = _ymin < _y7 ? _ymin : _y7;
  _ymin = _ymin < _y8 ? _ymin : _y8;

  float _zmin = _z1 < _z2 ? _z1 : _z2;
  _zmin = _zmin < _z3 ? _zmin : _z3;
  _zmin = _zmin < _z4 ? _zmin : _z4;
  _zmin = _zmin < _z5 ? _zmin : _z5;
  _zmin = _zmin < _z6 ? _zmin : _z6;
  _zmin = _zmin < _z7 ? _zmin : _z7;
  _zmin = _zmin < _z8 ? _zmin : _z8;

  float _xmax = _x1 > _x2 ? _x1 : _x2;
  _xmax = _xmax > _x3 ? _xmax : _x3;
  _xmax = _xmax > _x4 ? _xmax : _x4;
  _xmax = _xmax > _x5 ? _xmax : _x5;
  _xmax = _xmax > _x6 ? _xmax : _x6;
  _xmax = _xmax > _x7 ? _xmax : _x7;
  _xmax = _xmax > _x8 ? _xmax : _x8;

  float _ymax = _y1 > _y2 ? _y1 : _y2;
  _ymax = _ymax > _y3 ? _ymax : _y3;
  _ymax = _ymax > _y4 ? _ymax : _y4;
  _ymax = _ymax > _y5 ? _ymax : _y5;
  _ymax = _ymax > _y6 ? _ymax : _y6;
  _ymax = _ymax > _y7 ? _ymax : _y7;
  _ymax = _ymax > _y8 ? _ymax : _y8;

  float _zmax = _z1 > _z2 ? _z1 : _z2;
  _zmax = _zmax > _z3 ? _zmax : _z3;
  _zmax = _zmax > _z4 ? _zmax : _z4;
  _zmax = _zmax > _z5 ? _zmax : _z5;
  _zmax = _zmax > _z6 ? _zmax : _z6;
  _zmax = _zmax > _z7 ? _zmax : _z7;
  _zmax = _zmax > _z8 ? _zmax : _z8;

  m_bounding_box_max.Set(_xmax, _ymax, _zmax);
  m_bounding_box_min.Set(_xmin, _ymin, _zmin);
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
