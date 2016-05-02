#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"
#include "grid.h"

class Group : public Object3D
{
public:
	~Group();
	Group(int object_num);
	bool Intersect(const Ray &r, Hit &h, float tmin) const;
	void addObject(int index, Object3D *obj);
	Grid* EnableGridAccelerate(int nx, int ny, int nz, Material *m, bool visualize);
	void DisableGridAccelerate();
	void paint() const;
private:
	int m_nObjectNum;
	Object3D** m_pObjects;
	bool m_bGridAccelerate;
	Grid* m_pGrid;
};

#endif
