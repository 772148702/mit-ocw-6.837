#include "group.h"
#include <assert.h>
#include <memory.h>
#include "raytracing_status.h"

Group::~Group()
{
	m_nObjectNum = 0;
	if (m_pObjects)
	{
		delete[] m_pObjects;
		m_pObjects = NULL;
	}
}

Group::Group(int object_num)
{
	m_nObjectNum = object_num;
	m_pObjects = new Object3D*[object_num];
	memset(static_cast<void*>(m_pObjects), 0x00, sizeof(Object3D*) * object_num);

	m_bounding_box_min.Set(INFINITY, INFINITY, INFINITY);
	m_bounding_box_max.Set(-INFINITY, -INFINITY, -INFINITY);

	m_bGridAccelerate = false;
	m_pGrid = NULL;
}

void Group::addObject(int index, Object3D *obj)
{
	assert(index < m_nObjectNum);
	assert(index >= 0);
	m_pObjects[index] = obj;
	const Vec3f _obj_bounding_box_min = obj->GetBoundingBoxMin();
	const Vec3f _obj_bounding_box_max = obj->GetBoundingBoxMax();

	float _min_x = (m_bounding_box_min.x() < _obj_bounding_box_min.x()) ? m_bounding_box_min.x() : _obj_bounding_box_min.x();
	float _min_y = (m_bounding_box_min.y() < _obj_bounding_box_min.y()) ? m_bounding_box_min.y() : _obj_bounding_box_min.y();
	float _min_z = (m_bounding_box_min.z() < _obj_bounding_box_min.z()) ? m_bounding_box_min.z() : _obj_bounding_box_min.z();

	float _max_x = (m_bounding_box_max.x() > _obj_bounding_box_max.x()) ? m_bounding_box_max.x() : _obj_bounding_box_max.x();
	float _max_y = (m_bounding_box_max.y() > _obj_bounding_box_max.y()) ? m_bounding_box_max.y() : _obj_bounding_box_max.y();
	float _max_z = (m_bounding_box_max.z() > _obj_bounding_box_max.z()) ? m_bounding_box_max.z() : _obj_bounding_box_max.z();

	m_bounding_box_min.Set(_min_x, _min_y, _min_z);
	m_bounding_box_max.Set(_max_x, _max_y, _max_z);

	DisableGridAccelerate();
}

bool Group::Intersect(const Ray &r, Hit &h, float tmin) const
{
	bool result = false;

	if (m_bGridAccelerate) {
		assert(m_pGrid);
		result = m_pGrid->Intersect(r, h, tmin);
	}
	else {
		for (int _i = 0; _i < m_nObjectNum; _i++)
		{
			assert(m_pObjects[_i] != NULL);
			RayTracingStats::IncrementNumIntersections();
			result |= m_pObjects[_i]->Intersect(r, h, tmin);
		}
	}

	return result;
}

void Group::paint() const
{
	for (int _i = 0; _i < m_nObjectNum; _i++)
	{
		assert(m_pObjects[_i] != NULL);
		m_pObjects[_i]->paint();
	}
}

Grid* Group::EnableGridAccelerate(int nx, int ny, int nz, Material *m, bool visualize) {
	m_bGridAccelerate = true;
	assert(m_pGrid == NULL);

	m_pGrid = new Grid(m_bounding_box_min, m_bounding_box_max, nx, ny, nz, m);
	m_pGrid->SetVisualbility(visualize);

	for (int _i = 0; _i < m_nObjectNum; _i++)
	{
		Object3D* _pObj = m_pObjects[_i];
		assert(_pObj != NULL);

		Group * _pGroup = dynamic_cast<Group *>(_pObj);
		if (_pGroup != NULL) {
			m_pGrid->SetVisualbility(false); // hide outter grid
			_pObj = _pGroup->EnableGridAccelerate(nx, ny, nz, m, visualize);
		}
		m_pGrid->addObject(_pObj);
	}

	return m_pGrid;
}

void Group::DisableGridAccelerate() {
	m_bGridAccelerate = false;

	for (int _i = 0; _i < m_nObjectNum; _i++)
	{
		Object3D* _pObj = m_pObjects[_i];
		if (_pObj == NULL) continue;

		Group * _pGroup = dynamic_cast<Group *>(_pObj);
		if (_pGroup) {
			_pGroup->DisableGridAccelerate();
		}
	}

	delete m_pGrid;
}