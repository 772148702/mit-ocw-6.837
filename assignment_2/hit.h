#ifndef _HIT_H
#define _HIT_H

#include "vectors.h"

//class Material

// ====================================================================
// ====================================================================

class Hit {
  
public:

  // CONSTRUCTOR & DESTRUCTOR
  Hit(const float _t, const Vec3f c, const Vec3f n) { t = _t; color = c; normal = n; }
  Hit(const Hit &h) { t = h.t; color = h.color; normal = h.normal; }
  ~Hit() {}

  // ACCESSORS
  float getT() const { return t; }
  Vec3f getColor() const { return color; }
  Vec3f getNormal() const { return normal; }
  
  // MODIFIER
  void set(const float _t, const Vec3f c, const Vec3f n) { t = _t; color = c; normal = n; }

private: 

  // REPRESENTATION
  float t;
  Vec3f color;
  //Material *material;
  Vec3f normal;

};

inline ostream &operator<<(ostream &os, const Hit &h)
{
  os << "Hit <" << h.getT() << ", " << h.getColor() << ", " << h.getNormal() << ">";
  return os;
}

// ====================================================================
// ====================================================================

#endif
