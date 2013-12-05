#ifndef KERNEL_H
#define KERNEL_H

#include <vector>
#include <vecmath.h>
#include "extra.h"
#include "FluidParticle.h"

class FluidSystem;

using namespace std;

class Kernel {
public:
	Kernel() : h(1), h2(1), h3(1), h6(1), h9(1), ph2(1), ph3(1), ph6(1), ph9(1), fs(NULL) {}
	Kernel(FluidSystem* fs);
	Kernel(FluidSystem* fs, float h);
	void updateDensities();
	vector<FluidParticle> computeNeighbors(int index);
	float computeMassDensity(int index);
	Vector3f computeInternalPressure(int index);
	Vector3f computeViscosity(int index);
	Vector3f computeInternalForces(int index);
	Vector3f computeSurfaceTensionForce(int index);
	Vector3f computeNormal(int index);
	
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
