#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>

#include "FluidParticle.h"


using namespace std;
class Kernel;
class ParticleSystem
{
public:

	ParticleSystem(int numParticles=0);

	int m_numParticles;
	float viscosity;
	float restitution;
	float mass;
	Kernel* kernel;
	Vector3f minPoint;
	Vector3f maxPoint;
	vector<FluidParticle> particles;
	
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
			Vector3f force = kernel->computeInternalForce(i);
			force = force + p.density * Vector3f(0,-9.8,0);
			
			//	divide sum by mass-density
			rho = kernel->computeMassDensity(p);
			force = (1.0 / rho) * force;
		
		
			//	perform the collision check
			//vel = handleCollision(p, vel);
			F.push_back(vel);
			F.push_back(force);
		}
		return F;
	}

	Vector3f handleCollisions(){
		for(int i = 0; i < particles.size(); i++){
			float& x = particles[i].position[0];
			float& y = particles[i].position[1];
			float& z = particles[i].position[2];
			Vector3f velocity = m_vVecState[2*i+1];

			if( x > maxPoint[0] ){
				velocity[0] = 0.0f;
				x = maxPoint[0]; 
			}
			
			if( y > maxPoint[1] ){
				velocity[1] = 0.0f;
				y = maxPoint[1];
			}
			
			if( z > maxPoint[2] ){
				velocity[2] = 0.0f;
				z = maxPoint[2];
			}

			if( minPoint[0] <= x){
				velocity[0] = 0.0f;
				x = minPoint[0]; 
			}

			if( minPoint[1] <= y ){
				velocity[1] = 0.0f;
				y = minPoint[2];
			}

			if( minPoint[2] <= z ){
				velocity[2] = 0.0f;
				z = minPoint[2];
			}
		}
	};

	// getter method for the system's state
	vector<Vector3f> getState(){ return m_vVecState; };
	
	// setter method for the system's state
	void setState(const vector<Vector3f>  & newState) { m_vVecState = newState; };
	
	virtual void draw();
	
protected:

	vector<Vector3f> m_vVecState;
	
	
};

#endif
