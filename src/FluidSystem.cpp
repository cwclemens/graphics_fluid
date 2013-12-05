#include <iostream>
#include <vector>
#include <vecmath.h>
#include "FluidParticle.h"
#include "FluidSystem.h"

using namespace std;

const float EPSILON = 0.0f;

vector<Vector3f> FluidSystem::evalF(vector<Vector3f> state){
	//for each particle
	Vector3f vel;
	
	float rho;
	vector<Vector3f> F;
	for(int i = 0; i < particles.size(); i++){
		FluidParticle p = particles[i];
		vel = p.velocity;
		//	compute the sum of forces
		Vector3f force = kernel.computeInternalForces(i);
		force = force + p.density * Vector3f(0,-9.8,0);
		cout << "force";
		force.print();
		//	divide sum by mass-density
		rho = p.density;
		force = force / rho;

		//cout << "force ";
		//force.print();
		//	perform the collision check
		//vel = handleCollision(p, vel);
		F.push_back(vel / 10.0f);
		F.push_back(force / 10.0f);
	}
	return F;
}

void FluidSystem::handleCollisions(){
	for(int i = 0; i < particles.size(); i++){
		float& x = particles[i].position[0];
		float& y = particles[i].position[1];
		float& z = particles[i].position[2];
		Vector3f& velocity = particles[i].velocity;

		if( x > maxPoint[0] ){
			cout << "collision at max x" << endl;
			m_vVecState[2*i + 1][0] *= -restitution;
			m_vVecState[2*i][0] = maxPoint[0] - EPSILON; 
		}
		
		if( y > maxPoint[1] ){
			cout << "collision at max y" << endl;
			m_vVecState[2*i + 1][1] *= -restitution;
			m_vVecState[2*i][1] = maxPoint[1] - EPSILON;
		}
		
		if( z > maxPoint[2] ){
			cout << "collision at max z" << endl;
			m_vVecState[2*i + 1][2] *= -restitution;
			m_vVecState[2*i][2] = maxPoint[2] - EPSILON;
		}

		if(x < minPoint[0]){
			cout << "collision at min x" << endl;
			m_vVecState[2*i + 1][0] *= -restitution;
			m_vVecState[2*i][0] = minPoint[0] + EPSILON; 
		}

		if(y < minPoint[1]){
			cout << "collision at min y" << endl;
			m_vVecState[2*i + 1][1] *= -restitution;
			m_vVecState[2*i][1] = minPoint[2] + EPSILON;
		}

		if(z < minPoint[2]){
			cout << "collision at min z" << endl;
			m_vVecState[2*i + 1][2] *= -restitution;
			m_vVecState[2*i][2] = minPoint[2] + EPSILON;
		}
	}
}

void FluidSystem::printSystem(){
	for(int i = 0; i < m_numParticles; i++){
		//particle
		particles[i].position.print();
		particles[i].velocity.print();
		cout << "density " << particles[i].density << endl;
		cout << "internal pressure " << particles[i].pressure << endl;
		m_vVecState[2*i].print();
		m_vVecState[2*i + 1].print();
		cout << endl;
	}

	cout << endl;
}

void FluidSystem::draw() {
	
	for (int i = 0; i < m_numParticles; i++) {
		particles[i].draw();
	}
}

void FluidSystem::prestep() {
	kernel.updateDensities();
}

void FluidSystem::poststep() {
	printSystem();
	handleCollisions();
	updateParticles();
}

void FluidSystem::updateParticles(){
	for(int i = 0; i < m_numParticles; i++){
		particles[i].position = m_vVecState[2*i];
		particles[i].velocity = m_vVecState[2*i + 1];
	}
}

