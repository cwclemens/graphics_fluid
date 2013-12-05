
#include "pendulumSystem.h"
#include <iostream>
using namespace std;

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		m_vVecState.push_back(Vector3f(0,(m_numParticles-i)/2.0,0));
		m_vVecState.push_back(Vector3f(0,0,0));
	}
}

Vector3f PendulumSystem::sumForces(Vector3f position, Vector3f velocity, Vector3f front_position, Vector3f behind_position){
	Vector3f F_front;
	Vector3f F_back;
	int mass = 1;
	int k = 1;
	int r = 2;
	Vector3f Fg = mass * Vector3f(0,-1,0);
	
	Vector3f Fd = -1.0 * k * velocity;

	//Computing the force from (i-1, i)
	Vector3f d = front_position - position;
	float length = d.abs();
	
	
	if(length == 0){
		F_front = Vector3f(0,0,0);
	}else {
		float scalar = -1.0 * k * (length - r) / length;
		F_front = scalar * d;
	}
	//Computing the force from (i, i+1)
	d = position - behind_position;
	length = d.abs();
	
	
	if(length == 0){
		F_back = Vector3f(0,0,0);
	}else {
		float scalar = -1.0 * k * (length - r) / length;
		F_back = scalar * d;
	}

	return (1.0/mass) * (Fg + Fd - F_front + F_back);
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

	for(int i = 0; i < m_numParticles; i++){
		int pos = 2 * i;
		int vel = 2 * i + 1;
		if(i == 0){
			f.push_back(Vector3f(0,0,0));
			f.push_back(Vector3f(0,0,0));
		}else if(i == m_numParticles -1){
			f.push_back(state[vel]);
			//self_pos, self_vel, bfr_pos, self_pos
			f.push_back(PendulumSystem::sumForces(state[pos], state[vel], state[pos-2], state[pos]));
		}else{
			f.push_back(state[vel]);
			f.push_back(PendulumSystem::sumForces(state[pos], state[vel], state[pos-2], state[pos+2]));
		}
	}

	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = m_vVecState[2*i];
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
