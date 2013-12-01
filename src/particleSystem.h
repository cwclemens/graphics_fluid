#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>

using namespace std;

class ParticleSystem
{
public:

	ParticleSystem(int numParticles=0);

	int m_numParticles;
	float viscosity;
	float resitution;
	Kernel kernel;
	Vector3f minPoint;
	Vector3f maxPoint;
	
	// for a given state, evaluate derivative f(X,t)
	virtual vector<Vector3f> evalF(vector<Vector3f> state){
		//for each particle
		Vector3f vel;
		FluidParticle p;
		float rho;
		vector<Vector3f> F;
		for(int i = 0; i < particles.size(); i++){
			p = particles[i];
			vel = state[2*i+1];
			
			//	compute the sum of forces
			Vector3f force = kernel.computeInternalForce(p);
			force = force + Vector3f(0,-9.8,0);
			
			//	divide sum by mass-density
			rho = kernel.computeMassDensity(p);
			force = (1.0 / rho) * force;
		
		
			//	perform the collision check
			vel = handleCollision(p, vel);
			F.push_back(vel);
			F.push_back(force);
		}
		return F;
	}

	Vector3f handleCollision(FluidParticle particle, Vector3f velocity){
		float x = particle.location[0];
		if( !(minPoint[0] <= x && x < maxPoint[0]) ){
			velocity[0] = 0.0f;
		}

		float y = particle.location[1];
		if( !(minPoint[1] <= y && y < maxPoint[1]) ){
			velocity[1] = 0.0f;
		}

		float z = particle.location[2];
		if( !(minPoint[2] <= z && z < maxPoint[2]) ){
			velocity[2] = 0.0f;
		}

		return velocity;
	};

	// getter method for the system's state
	vector<Vector3f> getState(){ return m_vVecState; };
	
	// setter method for the system's state
	void setState(const vector<Vector3f>  & newState) { m_vVecState = newState; };
	
	virtual void draw();
	
protected:

	vector<Vector3f> m_vVecState;
	vector<FluidParticle> particles;
	
};

#endif
