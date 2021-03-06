#include "perspective_camera.h"
#include "matrix.h"
#include <assert.h>
#include <GL/freeglut.h>

PerspectiveCamera::PerspectiveCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float angle)
{
  m_center = center;
  m_direction = direction;
  m_direction.Normalize();
  m_up = up;
  m_up = m_up - m_direction * m_up.Dot3(m_direction);
  m_up.Normalize();
  Vec3f::Cross3(m_horizontal, m_direction, m_up);
  m_angle = angle;
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
  float size = tan(m_angle / 2.0f) * 2.0f;
  Vec3f direction =  m_horizontal * (point.x() - 0.5) * size + m_up * ( point.y() - 0.5) * size + m_direction;
  return Ray(direction, m_center);
}

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_angle*180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void)
{
	gluLookAt(m_center.x(), m_center.y(), m_center.z(),
		m_center.x() + m_direction.x(), m_center.y() + m_direction.y(), m_center.z() + m_direction.z(),
		m_up.x(), m_up.y(), m_up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist)
{
	m_center += m_direction*dist;

	// ===========================================
	// ASSIGNMENT 5: Fix any other affected values
	// ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, m_direction, m_up);
	horizontal.Normalize();

	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, m_direction);

	m_center += horizontal*dx + screenUp*dy;

	// ===========================================
	// ASSIGNMENT 5: Fix any other affected values
	// ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, m_direction, m_up);
	horizontal.Normalize();

	// Don't let the model flip upside-down (There is a singularity
	// at the poles when 'up' and 'direction' are aligned)
	float tiltAngle = acos(m_up.Dot3(m_direction));
	if (tiltAngle - ry > 3.13)
		ry = tiltAngle - 3.13;
	else if (tiltAngle - ry < 0.01)
		ry = tiltAngle - 0.01;

	Matrix rotMat = Matrix::MakeAxisRotation(m_up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

	rotMat.Transform(m_center);
	rotMat.TransformDirection(m_direction);
	m_direction.Normalize();

	// ===========================================
	// ASSIGNMENT 5: Fix any other affected values
	// ===========================================


}

