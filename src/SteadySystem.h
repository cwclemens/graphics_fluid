#ifndef STEADYSYSTEM_H
#define STEADYSYSTEM_H

#include "extra.h"
#include <vector>

#include "FluidSystem.h"

using namespace std;


class SteadySystem: public FluidSystem 
{
public:
	SteadySystem() {
		minPoint = Vector3f(0);
		maxPoint = Vector3f(1,4,1);
		viscosity = 3.0f;
		restitution = 0.2;
		// original 0.015625f
		mass = 700.0f;
		//original: 0.0375
		kernel = Kernel(this, 0.1);
		m_numParticles = 400;
		stiffness = 1.0;
		threshold = 7.065;
		surfaceTension = 0.0728;
		restDensity = 100.0;

		float r = 0.02f;
		float d = 2*r;
		
		for (int i=0; i<m_numParticles; i++){
			Vector3f position = Vector3f(r+(i%4)*d, r+(i/16)*d, r+((i/4)%4)*d);
			m_vVecState.push_back(position);
			m_vVecState.push_back(Vector3f(0));
			particles.push_back(FluidParticle(position, Vector3f()));
		}
	}

protected:
	
	 
};

#endif
