#ifndef _GRID_H_
#define _GRID_H_


#include <vector>
#include "object3d.h"
#include "marching_info.h"
#include "object3dvector.h"

using namespace std;

class Grid : public Object3D
{
public:
	Grid(Vec3f min, Vec3f max,
		int nx, int ny, int nz, Material *m);
	void rasterizeSphere(Vec3f center, float radius);
	bool Intersect(const Ray &r, Hit &h, float tmin) const;
	void initializeRayMarch(MarchingInfo &mi,
		const Ray &r, float tmin) const;
	bool SetVisualbility(bool visual);
	void addObject(Object3D *obj);
private:
	Vec3f m_size;
	int m_nx;
	int m_ny;
	int m_nz;
	Material *m_pM;

	float m_grid_x;
	float m_grid_y;
	float m_grid_z;

	bool m_bVisual;

	vector<bool> m_is_voxel_opaque;
	vector<Object3DVector> m_object_list;
private:
	void print_statistics();
};

#endif
