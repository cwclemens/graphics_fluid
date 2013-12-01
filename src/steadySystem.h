#ifndef STEADYSYSTEM_H
#define STEADYSYSTEM_H

#include "extra.h"
#include <vector>

#include "particleSystem.h"

using namespace std;

class SteadySystem: public ParticleSystem {
	public:
	SteadySystem() {
		minPoint = Vector3f(0);
		maxPoint = Vector3f(.1);
		viscosity = 3.5f;
		restitution = 0;
		mass = 0.015625f;
		kernel = Kernel(this, 0.0375);
		m_numParticles = 32;
		
		for (int i=0; i<m_numParticles; i++) {
			particles.push_back(FluidParticle(Vector3f(r+(i%4)*d, r+((i/4)%4)*d, r+(i/16)*d), Vector3f(0)));
		}
	};

	protected:
	float r = 0.0125f;
	float d = 2*r;
	 
};

#endif
