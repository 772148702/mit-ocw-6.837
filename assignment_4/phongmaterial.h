///////////////////////////////////////////////////////////
//  phongmaterial.h
//  Implementation of the Class PhongMaterial
//  Created on:      01-5-2012 19:03:29
//  Original author: HOME
///////////////////////////////////////////////////////////

#if !defined(EA_170957E7_0E1C_419e_98F2_5438890B7840__INCLUDED_)
#define EA_170957E7_0E1C_419e_98F2_5438890B7840__INCLUDED_

#include "vectors.h"
#include "material.h"

class PhongMaterial : public Material
{
private:
	Vec3f m_specularColor;
	float m_exponent;

public:
	PhongMaterial();
	virtual ~PhongMaterial();

	PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent, const Vec3f& transparentColor, const Vec3f& reflectiveColor, float indexOfRefraction);
	Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const;

	void glSetMaterial() const;
};
#endif // !defined(EA_170957E7_0E1C_419e_98F2_5438890B7840__INCLUDED_)
