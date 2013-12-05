
#include <vector>
#include <vecmath.h>
#include "FluidParticle.h"
#include "FluidSystem.h"

using namespace std;

vector<Vector3f> FluidSystem::evalF(vector<Vector3f> state){
	//for each particle
	Vector3f vel;
	
	float rho;
	vector<Vector3f> F;
	for(int i = 0; i < particles.size(); i++){
		FluidParticle p = particles[i];
		vel = state[2*i+1];

		//	compute the sum of forces
		Vector3f force = kernel.computeInternalForces(i);
		force = force + p.density * Vector3f(0,-9.8,0);
		
		//	divide sum by mass-density
		rho = kernel.computeMassDensity(i);
		force = force * (1.0 / rho);
	
	
		//	perform the collision check
		//vel = handleCollision(p, vel);
		F.push_back(vel);
		F.push_back(force);
	}
	return F;
}

void FluidSystem::handleCollisions(){
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

		if(x < minPoint[0]){
			velocity[0] = 0.0f;
			x = minPoint[0]; 
		}

		if(y < minPoint[1]){
			velocity[1] = 0.0f;
			y = minPoint[2];
		}

		if(z < minPoint[2]){
			velocity[2] = 0.0f;
			z = minPoint[2];
		}
	}
}

void FluidSystem::draw() {
	for (int i; i<m_numParticles; i++) {
		particles[i].draw();
	}
}

void FluidSystem::prestep() {
	kernel.updateDensities();
}

void FluidSystem::poststep() {
	handleCollisions();
	updatePositions();
}

void FluidSystem::updatePositions(){
	for(int i = 0; i < m_numParticles; i++){
		particles[i].position = m_vVecState[2*i];
	}
}

