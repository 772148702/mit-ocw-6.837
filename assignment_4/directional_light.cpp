#include "GL/freeglut.h"
#include "directional_light.h"

DirectionalLight::DirectionalLight() {
	direction = Vec3f(0, 0, 0);
	color = Vec3f(1, 1, 1);
}

DirectionalLight::~DirectionalLight() {}

DirectionalLight::DirectionalLight(const Vec3f &d, const Vec3f &c) {
	direction = d; direction.Normalize();
	color = c;
}

// VIRTUAL METHOD
void DirectionalLight::getIllumination(const Vec3f &p, Vec3f &dir, Vec3f &col) const {
	// the direction to the light is the opposite of the
	// direction of the directional light source
	dir = direction * (-1.0f);
	col = color;
}

// ====================================================================
// Create an OpenGL directional light source with the appropriate
// color and position 
// ====================================================================

void DirectionalLight::glInit(int id) const {
	GLenum glLightID;
	switch (id) {
	case 0: glLightID = GL_LIGHT0; break;
	case 1: glLightID = GL_LIGHT1; break;
	case 2: glLightID = GL_LIGHT2; break;
	case 3: glLightID = GL_LIGHT3; break;
	case 4: glLightID = GL_LIGHT4; break;
	case 5: glLightID = GL_LIGHT5; break;
	case 6: glLightID = GL_LIGHT6; break;
	case 7: glLightID = GL_LIGHT7; break;
	default: glLightID = GL_LIGHT7;
	}

	// Set the last component of the position to 0 to indicate
	// a directional light source
	GLfloat glPosition[4];
	glPosition[0] = -direction.x();
	glPosition[1] = -direction.y();
	glPosition[2] = -direction.z();
	glPosition[3] = 0.0;

	GLfloat glColor[4];
	glColor[0] = color.r();
	glColor[1] = color.g();
	glColor[2] = color.b();
	glColor[3] = 1.0;

	glLightfv(glLightID, GL_POSITION, glPosition);
	glLightfv(glLightID, GL_DIFFUSE, glColor);
	glLightfv(glLightID, GL_SPECULAR, glColor);
	glEnable(glLightID);
}

