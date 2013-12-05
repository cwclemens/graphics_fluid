#ifndef KERNEL_H
#define KERNEL_H

#include <vector>
#include <vecmath.h>
#include "extra.h"

class FluidSystem;

using namespace std;

class Kernel {
public:
	Kernel(ParticleSystem* fs);
	Kernel(ParticleSystem* fs, float h);
	void updateDensities();
	vector<FluidParticle> computeNeighbors(int index);
	float computeMassDensity(int index);
	Vector3f computeInternalPressure(int index);
	Vector3f computeViscosity(int index);
	Vector3f computeInternalForces(int index);

	
protected:


	// Weight functions

	float gaussianWeight(Vector3f r, float h);
	float gaussianWeight(Vector3f r);
	
	float defaultWeight(Vector3f r, float h);
	float defaultWeight(Vector3f r);
	Vector3f gradDefaultWeight(Vector3f r, float h);
	Vector3f gradDefaultWeight(Vector3f r);
	float laplaceDefaultWeight(Vector3f r, float h);
	float laplaceDefaultWeight(Vector3f r);

	float pressureWeight(Vector3f r, float h);
	float pressureWeight(Vector3f r);
	Vector3f gradPressureWeight(Vector3f r, float h);
	Vector3f gradPressureWeight(Vector3f r);
	float laplacePressureWeight(Vector3f r, float h);
	float laplacePressureWeight(Vector3f r);

	float viscosityWeight(Vector3f r, float h);
	float viscosityWeight(Vector3f r);
	Vector3f gradViscosityWeight(Vector3f r, float h);
	Vector3f gradViscosityWeight(Vector3f r);
	float laplaceViscosityWeight(Vector3f r, float h);
	float laplaceViscosityWeight(Vector3f r);

	// Variables
	FluidSystem* fs;
	float h, h2, h3, h6, h9, ph2, ph3, ph6, ph9;
};

#endif
