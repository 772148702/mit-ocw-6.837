///////////////////////////////////////////////////////////
//  raytracer.cpp
//  Implementation of the Class RayTracer
//  Created on:      01-5-2012 19:35:42
//  Original author: HOME
///////////////////////////////////////////////////////////

#include "raytracer.h"
#include "group.h"
#include "light.h"
#include "ray.h"
#include "hit.h"
#include "raytracing_status.h"


RayTracer::RayTracer(SceneParser *s, int max_bounces, float min_weight, bool cast_shadow) {
  m_pSceneParser = s;
  m_nMaxBounces = max_bounces;
  m_fMinWeight = min_weight;
  m_bCastShadow = cast_shadow;
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const {
  Group *group = m_pSceneParser->getGroup();
  Vec3f pixel = m_pSceneParser->getBackgroundColor();

  RayTracingStats::IncrementNumNonShadowRays();

  if (bounces <= m_nMaxBounces && weight >= m_fMinWeight) {
    if (group->Intersect(ray, hit, tmin)) {
		const Material *material = hit.getMaterial();
		assert(material);
		const float epsilon = 0.001f;

		// ambient component
		Vec3f color;
		pixel = material->getDiffuseColor();
		Vec3f ambient = m_pSceneParser->getAmbientLight();
		pixel.Scale(ambient);

		const Vec3f p = hit.getIntersectionPoint();
		int light_num = m_pSceneParser->getNumLights();

		// light contribute
		for ( int k = 0; k < light_num; k++) {
		  Light *light = m_pSceneParser->getLight(k);
		  Vec3f dir_to_light;
		  Vec3f col;
		  light->getIllumination(p, dir_to_light, col);

		  if (m_bCastShadow) {
			  RayTracingStats::IncrementNumShadowRays();
			  Ray ray2(dir_to_light, p);
			  Hit hit2(INFINITY, NULL, Vec3f());

			  // shadow test
			  if (!group->Intersect(ray2, hit2, epsilon)) {
				pixel += material->Shade(ray, hit, dir_to_light, col);
			  }
		  }
		  else {
			  pixel += material->Shade(ray, hit, dir_to_light, col);
		  }
		}

		// Handle the case when trace from the inside of the surface. (as double sided material)
		float index_t;
		Vec3f n = hit.getNormal();
		const Vec3f ray_dir = ray.getDirection();
		if (n.Dot3(ray_dir * -1.0f) > 0) {
			index_t = material->getIndexOfRefraction();
		} else {
			index_t = 1.0f;
			n.Negate();
		}

		// reflection
		color = material->getReflectiveColor();

		const float cosThita = - (n.Dot3(ray_dir) / (n.Length() * ray_dir.Length()));
	    assert(cosThita >= 0.0f);
	    Vec3f ref_rate = color + (Vec3f(1.0f, 1.0f, 1.0f) - color) * pow((1 - cosThita), 5.0f);
	    assert(ref_rate.r() >= 0 && ref_rate.g() >= 0 && ref_rate.b() >= 0);
	    assert(ref_rate.r() <= 1.0f && ref_rate.g() <= 1.0f && ref_rate.b() <= 1.0f);
	    if (ref_rate.Length() > 0.0f) {
		  Vec3f ref_dir = mirrorDirection(n, ray_dir);
		  Ray ray3(ref_dir, p);
		  Hit hit3(INFINITY, NULL, Vec3f());
		  Vec3f ref_color = traceRay(ray3, epsilon, bounces + 1, weight * ref_rate.Length() / sqrt(3.0f), indexOfRefraction, hit3);
		  ref_color.Scale(ref_rate.r(), ref_rate.g(), ref_rate.b());
		  ref_color.Scale(color);
		  pixel += ref_color;
	    }

		// reflaction
		color = material->getTransparentColor();
		Vec3f rfa_dir;

		bool is_transparent = transmittedDirection(n, ray_dir, indexOfRefraction, index_t, rfa_dir);
		if (is_transparent) {
		  Ray ray4(rfa_dir, p);
		  Hit hit4(INFINITY, NULL, Vec3f());
		  Vec3f rfa_color = traceRay(ray4, epsilon, bounces + 1, weight * color.Length() / sqrt(3.0f), index_t, hit4);
		  rfa_color.Scale(color);
	      pixel += rfa_color;
		}
	}
  } else {
	  pixel = Vec3f(0.0f, 0.0f, 0.0f);
  }

  return pixel;
}

Vec3f RayTracer::mirrorDirection(const Vec3f& normal, const Vec3f& incoming) const  {
  Vec3f dir = (incoming - normal * 2.0f * incoming.Dot3(normal));
  dir.Normalize();
  return dir;
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted) const {
  bool result = false;
  float dot3;
  Vec3f n = normal;
  dot3 = n.Dot3(incoming * -1.0f);

  if (index_t > 0.0f) {
	float index_r = index_i / index_t;
	float tmp = 1.0f - pow(index_r, 2.0f) * (1.0f - pow(dot3, 2.0f));
	if (tmp >= 0) {
	  transmitted = n * static_cast<float>(index_r * dot3 - sqrt(tmp)) + incoming * index_r;
	  transmitted.Normalize();
	  result = true;
	}
  }

  return result;
}

RayTracer::~RayTracer(){

}
