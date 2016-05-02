#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vectors.h"

// ====================================================================
// ====================================================================

class Light {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Light() {}
  virtual ~Light() {}

  // VIRTUAL METHOD
  virtual void getIllumination (const Vec3f &p, Vec3f &dir, Vec3f &col) const = 0;
  virtual void glInit(int id) const = 0;
};

#endif

