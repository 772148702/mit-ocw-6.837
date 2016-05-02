///////////////////////////////////////////////////////////
//  material.h
//  Implementation of the Class Material
//  Created on:      01-5-2012 19:02:24
//  Original author: HOME
///////////////////////////////////////////////////////////

#if !defined(EA_23C249E2_79B4_4f59_A750_EE69A40B9604__INCLUDED_)
#define EA_23C249E2_79B4_4f59_A750_EE69A40B9604__INCLUDED_

#include "vectors.h"
#include "ray.h"
#include "hit.h"

class Material
{
private:
	Vec3f m_diffuseColor;
	Vec3f m_transparentColor;
	Vec3f m_reflectiveColor;
	float m_indexOfRefraction;

public:
	Material() {};
	virtual ~Material() {};

	Material(const Vec3f& diffuseColor, const Vec3f& transparentColor, const Vec3f& reflectiveColor, float indexOfRefraction) {
		m_diffuseColor = diffuseColor;
		m_transparentColor = transparentColor;
		m_reflectiveColor = reflectiveColor;
		m_indexOfRefraction = indexOfRefraction;
	};
	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const =0;

    Vec3f getDiffuseColor() const {	return m_diffuseColor; };
	Vec3f getTransparentColor() const { return m_transparentColor; };
	Vec3f getReflectiveColor() const { return m_reflectiveColor; };
	float getIndexOfRefraction() const { return m_indexOfRefraction; };
};
#endif // !defined(EA_23C249E2_79B4_4f59_A750_EE69A40B9604__INCLUDED_)
