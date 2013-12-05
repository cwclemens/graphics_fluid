#ifndef DAMSYSTEM_H
#define DAMSYSTEM_H

#include "extra.h"
#include <vector>
#include <cmath>

#include "FluidSystem.h"

using namespace std;


class DamSystem: public FluidSystem 
{
public:
	DamSystem() {
		minPoint = Vector3f(0);
		maxPoint = Vector3f(2,2,2);
		viscosity = 3.0f;
		restitution = 0.2;
		// original 0.015625f
		mass = 700.0f;
		//original: 0.0375
		kernel = Kernel(this, 0.1);
		m_numParticles = 500;
		stiffness = 1.0;
		threshold = 7.065;
		surfaceTension = 0.1728;
		restDensity = 100.0;

		Vector3f diagonal = maxPoint - minPoint;
		Vector3f setDiagonal = Vector3f(0.5*diagonal[0], 0.8*diagonal[1], diagonal[2]);
		float r = pow((setDiagonal[0] * setDiagonal[1] * setDiagonal[2])/m_numParticles, 1/3.0);		
		int nx = setDiagonal[0]/r;
		int ny = setDiagonal[1]/r;
		int nz = m_numParticles/(nx*ny) + (m_numParticles%(nx*ny)==0? 0 : 1);

		for (int y=0; y<ny; y++) {		
			for (int x=0; x<nx; x++) {
				for (int z=0; z<nz; z++) {
					if (nx*nz*y + nz*x + z+1 <= m_numParticles) {
						Vector3f position = minPoint + Vector3f(x*r, y*r, z*r);
						m_vVecState.push_back(position);
						m_vVecState.push_back(Vector3f(0));
						particles.push_back(FluidParticle(position, Vector3f()));
					}
				}
			}
		}
		
	}

protected:
	
	 
};

#endif
