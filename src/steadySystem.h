#ifndef STEADYSYSTEM_H
#define STEADYSYSTEM_H

#include "extra.h"
#include <vector>

#include "particleSystem.h"

using namespace std;

class SteadySystem: public ParticleSystem {
	public:
	 SteadySystem();
	 vector<Vector3f> evalF(vector<Vector3f> state);

	 void draw();
};

#endif