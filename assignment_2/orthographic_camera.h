#ifndef _ORTHOGRAPHIC_CAMERA_H_
#define _ORTHOGRAPHIC_CAMERA_H_

#include "camera.h"
#include "vectors.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size);
	Ray generateRay(Vec2f point);
protected:
	Vec3f m_center;
	Vec3f m_direction;
	Vec3f m_up;
	Vec3f m_horizontal;
	float m_size;
};

#endif
