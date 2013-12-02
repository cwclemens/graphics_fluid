#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include "extra.h"
#include <vector>

#include "particleSystem.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
	static Vector3f sumForces(Vector3f position, Vector3f velocity, Vector3f front_position, Vector3f behind_position);
	void draw();
	
};

#endif
