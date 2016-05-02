///////////////////////////////////////////////////////////
//  raytracer.h
//  Implementation of the Class RayTracer
//  Created on:      01-5-2012 19:35:42
//  Original author: HOME
///////////////////////////////////////////////////////////

#if !defined(EA_21E020A7_5358_492d_843D_D53D0ABC02DC__INCLUDED_)
#define EA_21E020A7_5358_492d_843D_D53D0ABC02DC__INCLUDED_

#include "scene_parser.h"
#include "hit.h"
#include "ray.h"

class RayTracer
{

public:
	RayTracer();
    RayTracer(SceneParser *s, int max_bounces, float min_weight);
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const;
	Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming) const;
	bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted) const;
	virtual ~RayTracer();

private:
	float m_fMinWeight;
	int m_nMaxBounces;
	SceneParser* m_pSceneParser;

};
#endif // !defined(EA_21E020A7_5358_492d_843D_D53D0ABC02DC__INCLUDED_)
