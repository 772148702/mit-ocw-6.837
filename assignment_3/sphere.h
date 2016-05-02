#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3d.h"
#include "material.h"

class Sphere : public Object3D
{
public:
	Sphere(const Vec3f &center, float radius, const Material *material);
	bool Intersect(const Ray &r, Hit &h, float tmin) const;
private:
	Vec3f m_center;
	float m_radius;
};

#endif
