#include "vectors.h"
#include "light.h"

class DirectionalLight : public Light {

public:

	// CONSTRUCTOR & DESTRUCTOR
	DirectionalLight();
	DirectionalLight(const Vec3f &d, const Vec3f &c);
	virtual ~DirectionalLight();

	// VIRTUAL METHOD
	void getIllumination(const Vec3f &p, Vec3f &dir, Vec3f &col) const;

	virtual void glInit(int id) const;

private:

	// REPRESENTATION
	Vec3f direction;
	Vec3f color;

};
