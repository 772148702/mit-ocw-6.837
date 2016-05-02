#ifndef _OBJECT_3D_VECTOR_H_
#define _OBJECT_3D_VECTOR_H_

#include "assert.h"
#include <vector>
class Object3D;

using namespace std;

typedef Object3D* Object3DPtr;

// ====================================================================
// ====================================================================

// Store an collection of Object3D.  The total number to be stored is
// not known initially.  Resize as necessary.

// ====================================================================
// ====================================================================

class Object3DVector {

public:

	// CONSTRUCTOR
	Object3DVector() {
	}

	// DESTRUCTOR
	~Object3DVector() {
		
	}

	// ACCESSORS
	int getNumObjects() { return objects.size(); }
	Object3D* getObject(int i) {
		assert(objects[i] != NULL);
		return objects[i];
	}

	// MODIFIERS
	void addObject(Object3D *o) {
		assert(o != NULL);
		objects.push_back(o);
	}

private:

	// REPRESENTATION
	vector<Object3DPtr> objects;
};

// ====================================================================
// ====================================================================

#endif

