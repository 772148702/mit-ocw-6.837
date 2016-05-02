///////////////////////////////////////////////////////////
//  phongmaterial.cpp
//  Implementation of the Class PhongMaterial
//  Created on:      01-5-2012 19:03:30
//  Original author: HOME
///////////////////////////////////////////////////////////

#include "phongmaterial.h"
#include "math.h"


PhongMaterial::PhongMaterial(){

}

PhongMaterial::~PhongMaterial(){

}


PhongMaterial::PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent, const Vec3f& transparentColor, const Vec3f& reflectiveColor, float indexOfRefraction)
	:Material(diffuseColor, transparentColor, reflectiveColor, indexOfRefraction) {
	m_specularColor = specularColor;
	m_exponent = exponent;
}


Vec3f PhongMaterial::Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const {
	Vec3f v = ray.getDirection() * -1.0f;
    Vec3f n = hit.getNormal();
	// Vec3f h = dirToLight + ray.getDirection() * -1.0f;
	// h.Normalize();
	float diffuse = dirToLight.Dot3(n);
	Vec3f r = n * 2.0 * n.Dot3(dirToLight) - dirToLight;
	float specular = v.Dot3(r);
	
	if (diffuse < 0.0f) diffuse = 0.0f;
	if (specular < 0.0f) specular = 0.0f;
	
	Vec3f color = this->getDiffuseColor();
	color = color * diffuse + m_specularColor * pow(specular, m_exponent);
	color.Scale(lightColor);
	
	return color;
}
