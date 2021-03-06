#ifndef _ORTHOGRAPHIC_CAMERA_H_
#define _ORTHOGRAPHIC_CAMERA_H_

#include "camera.h"
#include "vectors.h"

class OrthographicCamera : public Camera
{
private:
	Vec3f m_center;
	Vec3f m_direction;
	Vec3f m_horizontal;
	float m_size;
	Vec3f m_up;

public:
	OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size);
	Ray generateRay(Vec2f point);
	virtual void glInit(int w, int h);
	virtual void glPlaceCamera(void);
	virtual void dollyCamera(float dist);
	virtual void truckCamera(float dx, float dy);
	virtual void rotateCamera(float rx, float ry);
protected:
};

#endif
