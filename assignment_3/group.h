#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"

class Group : public Object3D
{
public:
	~Group();
	Group(int object_num);
	bool Intersect(const Ray &r, Hit &h, float tmin) const;
	void addObject(int index, Object3D *obj);
private:
	int m_nObjectNum;
	Object3D** m_pObjects;
};

#endif
