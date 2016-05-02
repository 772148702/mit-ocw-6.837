#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Ray.h"
#include "vectors.h"


class Camera
{
public:
	inline virtual ~Camera(){};
	virtual Ray generateRay(Vec2f point) = 0;
	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;
};

#endif
