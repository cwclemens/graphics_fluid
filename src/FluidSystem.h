#ifndef FLUIDSYSTEM_H
#define FLUIDSYSTEM_H

#include <vector>
#include <vecmath.h>

#include "ParticleSystem.h"
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

	void prestep();
	void poststep();
	
protected:
	FluidSystem() {
		float viscosity = 1;
		float restitution = 0;
		float mass = 1;
		Kernel kernel = Kernel(this);
		Vector3f minPoint = Vector3f(0);
		Vector3f maxPoint = Vector3f(1);
	}
	
};

#endif
