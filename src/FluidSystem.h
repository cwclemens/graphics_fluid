#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>

#include "FluidParticle.h"
#include "Kernel.h"


using namespace std;


class FluidSystem : public ParticleSystem
{
public:

	float viscosity;
	float restitution;
	float mass;
	Kernel kernel;
	Vector3f minPoint;
	Vector3f maxPoint;
	vector<FluidParticle> particles;
	
	// for a given state, evaluate derivative f(X,t)
	virtual vector<Vector3f> evalF(vector<Vector3f> state);

	Vector3f handleCollisions();
	
	virtual void draw();
	
protected:
	
	
};

#endif
