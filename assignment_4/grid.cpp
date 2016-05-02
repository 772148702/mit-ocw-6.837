#include "grid.h"
#include "raytracing_status.h"
#include "assert.h"
#include <float.h>
#include <iostream>
#include <map>

Grid::Grid(Vec3f min, Vec3f max,
	int nx, int ny, int nz, Material *m)
{
	m_bounding_box_min = min;
	m_bounding_box_max = max + FLT_EPSILON;
	m_nx = nx;
	m_ny = ny;
	m_nz = nz;
	m_pM = m;

	m_size = m_bounding_box_max - m_bounding_box_min;
	m_grid_x = m_size.x() / m_nx;
	m_grid_y = m_size.y() / m_ny;
	m_grid_z = m_size.z() / m_nz;

	m_is_voxel_opaque.assign(m_nx * m_ny * m_nz, false);
	m_object_list.resize(m_nx * m_ny * m_nz);
	m_bVisual = true;
}

void Grid::rasterizeSphere(Vec3f center, float radius)
{
	Vec3f _voxel;

	center -= m_bounding_box_min;

	for (int _i = 0; _i < m_nx; _i++) {
		float _x1 = (_i + 0.5f)*m_grid_x;
		for (int _j = 0; _j < m_ny; _j++) {
			float _y1 = (_j + 0.5f)*m_grid_y;
			for (int _k = 0; _k < m_nz; _k++) {
				float _z1 = (_k + 0.5f)*m_grid_z;
				_voxel.Set(_x1, _y1, _z1);
				if ((_voxel - center).Length() <= radius)
					m_is_voxel_opaque[(_i * m_ny + _j) * m_nz + _k] = true;
			}
		}
	}

	print_statistics();
}

bool Grid::Intersect(const Ray &r, Hit &h, float tmin) const
{
	bool result = false;

	MarchingInfo mi;
	initializeRayMarch(mi, r, tmin);

	if (mi.tmin < h.getT()) {
		vector<Object3DPtr> _obj_intersected;

		while (mi.i < m_nx && mi.j < m_ny && mi.k < m_nz &&
			mi.i >= 0 && mi.j >= 0 && mi.k >= 0) {
			RayTracingStats::IncrementNumGridCellsTraversed();

			if (m_is_voxel_opaque[(mi.i * m_ny + mi.j) * m_nz + mi.k]) {

				if (m_bVisual) {
					RayTracingStats::IncrementNumIntersections();
					h.set(mi.tmin, m_pM, mi.normal, r);
					result = true;
					break;
				}
				else {
					Object3DVector _obj_list = m_object_list[(mi.i * m_ny + mi.j) * m_nz + mi.k];
					for (int _i = 0; _i < _obj_list.getNumObjects(); _i++) {
						Object3DPtr _pObj = _obj_list.getObject(_i);
						if (!_pObj->GetIntersectFlag()) {
							RayTracingStats::IncrementNumIntersections();
							result |= _pObj->Intersect(r, h, tmin);
							_pObj->SetIntersectFlag(true);
							_obj_intersected.push_back(_pObj);
						}
					}
					if (result) {
						Vec3f _p = h.getIntersectionPoint();
						_p -= m_bounding_box_min;
						_p.Divide(m_grid_x, m_grid_y, m_grid_z);
						if (_p.x() >= mi.i && _p.x() < mi.i + 1 &&
							_p.y() >= mi.j && _p.y() < mi.j + 1 &&
							_p.z() >= mi.k && _p.z() < mi.k + 1) {
							break;
						}
					}
				}
			}

			mi.nextCell();
		}

		// Clear intersection flag
		for (vector<Object3DPtr>::iterator _pObj = _obj_intersected.begin(); _pObj != _obj_intersected.end(); ++_pObj) {
			(*_pObj)->SetIntersectFlag(false);
		}
	}

	return result;
}

void Grid::initializeRayMarch(MarchingInfo &mi,
	const Ray &r, float tmin) const 
{
	const Vec3f D = r.getDirection();
	const Vec3f O = r.getOrigin();

	float _tnear = tmin;
	float _tfar = INFINITY;
	Vec3f _n(0.0f, 0.0f, 0.0f);

	assert(_tnear >= 0);

	// x axy
	float _dx = D.x();
	float _t1x = (m_bounding_box_min.x() - O.x()) / _dx;
	float _t2x = (m_bounding_box_max.x() - O.x()) / _dx;
	if (_t1x > _t2x) { float _tmp = _t1x; _t1x = _t2x; _t2x = _tmp; }

	// y axy
	float _dy = D.y();
	float _t1y = (m_bounding_box_min.y() - O.y()) / _dy;
	float _t2y = (m_bounding_box_max.y() - O.y()) / _dy;
	if (_t1y > _t2y) { float _tmp = _t1y; _t1y = _t2y; _t2y = _tmp; }

	// z axy
	float _dz = D.z();
	float _t1z = (m_bounding_box_min.z() - O.z()) / _dz;
	float _t2z = (m_bounding_box_max.z() - O.z()) / _dz;
	if (_t1z > _t2z) { float _tmp = _t1z; _t1z = _t2z; _t2z = _tmp; }

	_tnear = (_t1x > _tnear) ? _t1x : _tnear;
	_tnear = (_t1y > _tnear) ? _t1y : _tnear;
	_tnear = (_t1z > _tnear) ? _t1z : _tnear;

	_tfar = (_t2x < _tfar) ? _t2x : _tfar;
	_tfar = (_t2y < _tfar) ? _t2y : _tfar;
	_tfar = (_t2z < _tfar) ? _t2z : _tfar;

	if (_tfar < _tnear) return; // missed

	mi.sign_x = _dx>0 ? 1 : -1;
	mi.sign_y = _dy>0 ? 1 : -1;
	mi.sign_z = _dz>0 ? 1 : -1;

	if (_tnear == _t1x) _n += Vec3f(-1.0f, 0.0f, 0.0f) * mi.sign_x;
	if (_tnear == _t1y) _n += Vec3f(0.0f, -1.0f, 0.0f) * mi.sign_y;
	if (_tnear == _t1z) _n += Vec3f(0.0f, 0.0f, -1.0f) * mi.sign_z;
	_n.Normalize();

	mi.tmin = _tnear;
	mi.dt_x = fabs(m_grid_x / _dx);
	mi.dt_y = fabs(m_grid_y / _dy);
	mi.dt_z = fabs(m_grid_z / _dz);
	if (_t1x > -INFINITY) {
		while (_t1x <= _tnear){ _t1x += mi.dt_x; }
		mi.tnext_x = _t1x;
	}
	if (_t1y > -INFINITY) {
		while (_t1y <= _tnear){ _t1y += mi.dt_y; }
		mi.tnext_y = _t1y;
	}
	if (_t1z> -INFINITY) {
		while (_t1z <= _tnear){ _t1z += mi.dt_z; }
		mi.tnext_z = _t1z;
	}
	Vec3f _rp = O + D * _tnear - m_bounding_box_min;
	_rp.Divide(m_grid_x, m_grid_y, m_grid_z);
	mi.i = static_cast<int>(_rp.x()); if (mi.sign_x < 0 && mi.i == m_nx) mi.i--;
	mi.j = static_cast<int>(_rp.y()); if (mi.sign_y < 0 && mi.j == m_ny) mi.j--;
	mi.k = static_cast<int>(_rp.z()); if (mi.sign_z < 0 && mi.k == m_nz) mi.k--;
	mi.normal = _n;
}

void Grid::print_statistics()
{
	cout << "Left-bottom coner of the grid: " << m_bounding_box_min << endl;
	cout << "Right-upper coner of the grid: " << m_bounding_box_max << endl;
	cout << "Grid diamention: " << m_nx << "*" << m_ny << "*" << m_nz << endl;
	cout << "Grid m_is_voxel_opaque dump: " << endl;
	for (int _i = 0; _i < m_nx; _i++) {
		for (int _j = 0; _j < m_ny; _j++) {
			for (int _k = 0; _k < m_nz; _k++) {
				cout << m_is_voxel_opaque[(_i * m_ny + _j) * m_nz + _k];
			}
			cout << endl;
		}
		cout << endl << endl;
	}
}

bool Grid::SetVisualbility(bool visual) {
	bool _old_visual = m_bVisual;
	m_bVisual = visual;
	return _old_visual;
}

void Grid::addObject(Object3D *obj) {
	assert(obj);
	Vec3f _bounding_min = obj->GetBoundingBoxMin() - m_bounding_box_min;
	Vec3f _bounding_max = obj->GetBoundingBoxMax() - m_bounding_box_min;

	_bounding_min.Divide(m_grid_x, m_grid_y, m_grid_z);
	_bounding_max.Divide(m_grid_x, m_grid_y, m_grid_z);

	int _start_i = floor(_bounding_min.x());
	int _start_j = floor(_bounding_min.y());
	int _start_k = floor(_bounding_min.z());
	int _end_i = ceil(_bounding_max.x());
	int _end_j = ceil(_bounding_max.y());
	int _end_k = ceil(_bounding_max.z());

	if (_start_i == _end_i) _end_i++;
	if (_start_j == _end_j) _end_j++;
	if (_start_k == _end_k) _end_k++;

	assert(_end_i <= m_nx && _end_j <= m_ny && _end_k <= m_nz);

	for (int _i = _start_i; _i < _end_i; _i++) {
		for (int _j = _start_j; _j < _end_j; _j++) {
			for (int _k = _start_k; _k < _end_k; _k++) {
				m_is_voxel_opaque[(_i * m_ny + _j) * m_nz + _k] = true;
				m_object_list[(_i * m_ny + _j) * m_nz + _k].addObject(obj);
			}
		}
	}
}

