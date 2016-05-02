#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "vectors.h"
#include "Ray.h"
#include "hit.h"
#include "material.h"

class Object3D
{
protected:
	Vec3f m_bounding_box_min;
	Vec3f m_bounding_box_max;
	const Material *m_material;
	bool  m_intersected = false;
public:
  inline virtual ~Object3D(){};
  inline void SetIntersectFlag(bool _intersected){ m_intersected = _intersected; };
  inline bool GetIntersectFlag() const { return m_intersected; };
  virtual bool Intersect(const Ray &r, Hit &h, float tmin) const  = 0;
  inline const Vec3f& GetBoundingBoxMin() const{ return m_bounding_box_min; };
  inline const Vec3f& GetBoundingBoxMax() const{ return m_bounding_box_max; };
  inline virtual void paint() const { m_material->glSetMaterial(); };
};

#endif
