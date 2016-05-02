#include "group.h"
#include <assert.h>
#include <memory.h>

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
}

void Group::addObject(int index, Object3D *obj)
{
	assert(index < m_nObjectNum);
	assert(index >= 0);
	m_pObjects[index] = obj;
}

bool Group::Intersect(const Ray &r, Hit &h, float tmin) const
{
	bool result = false;

	for (int i = 0; i < m_nObjectNum; i++)
	{
		assert(m_pObjects[i] != NULL);
		result |= m_pObjects[i]->Intersect(r, h, tmin);
	}

	return result;
}
