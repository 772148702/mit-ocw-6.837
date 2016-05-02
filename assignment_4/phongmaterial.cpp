///////////////////////////////////////////////////////////
//  phongmaterial.cpp
//  Implementation of the Class PhongMaterial
//  Created on:      01-5-2012 19:03:30
//  Original author: HOME
///////////////////////////////////////////////////////////

#include "phongmaterial.h"
#include "math.h"

#include <GL/freeglut.h>

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

// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

void PhongMaterial::glSetMaterial(void) const
{
	// Set the diffuse and ambient colors
	GLfloat diffuseArray[4];
	diffuseArray[0] = m_diffuseColor.x();
	diffuseArray[1] = m_diffuseColor.y();
	diffuseArray[2] = m_diffuseColor.z();
	diffuseArray[3] = 1.0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuseArray);

	// Set the specular color and exponent
	GLfloat specularArray[4];
	specularArray[0] = m_specularColor.x();
	specularArray[1] = m_specularColor.y();
	specularArray[2] = m_specularColor.z();
	specularArray[3] = 1.0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularArray);

	// GL uses the Blinn Torrance version of Phong so we need to scale
	// the exponent so it looks similar (NOTE: This is *not* a correct
	// mapping of Phong to Blinn-Torrance!!  There will be visual
	// differences between the GL version and the raytraced version!)
	float glexponent = float(m_exponent * 4.0f);
	if (glexponent > 128) glexponent = 128;
	if (glexponent < 0) glexponent = 0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
}

// ====================================================================
// ====================================================================
