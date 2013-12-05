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
	float stiffness;
	float restDensity;
	Kernel kernel;
	Vector3f minPoint;
	Vector3f maxPoint;
	vector<FluidParticle> particles;
	
	// for a given state, evaluate derivative f(X,t)
	virtual vector<Vector3f> evalF(vector<Vector3f> state);

	void handleCollisions();
	
	virtual void draw();

	void prestep();
	void poststep();
	void printSystem();
	void updateParticles();
	
protected:

};

#endif
