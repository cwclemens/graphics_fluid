#include "ClothSystem.h"
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

vector<Vector2f> flex_coords;
//TODO: Initialize here
ClothSystem::ClothSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles * numParticles;
	for(int i = 0; i < numParticles; i++){
		for(int j = 0; j < numParticles; j++){
			m_vVecState.push_back(Vector3f(-1*i/2.0,-1*j/2.0,0));
			m_vVecState.push_back(Vector3f(0,0,0));

			//pre computing flex coordinates
			vector<Vector2f> neighbors = getFlex(i, j);
			if(neighbors.size() == 3){
				flex_coords.push_back(Vector2f(i,j));
				for(int n = 0; n < neighbors.size(); n++){
					flex_coords.push_back(neighbors[n]);
				}
			}
		}
	}




}

vector<Vector2f> ClothSystem::getNeighbors(int i, int j){
	int numParticles = int(sqrt(m_numParticles));
	Vector3f new_i = Vector3f(i-1, i, i + 1);
	Vector3f new_j = Vector3f(j-1, j, j + 1);
	vector<Vector2f> neighbors;

	if(i > 0){
		neighbors.push_back(Vector2f(i-1, j));
	}

	if(i < numParticles-1){
		neighbors.push_back(Vector2f(i+1,j));
	}

	if(j > 0){
		neighbors.push_back(Vector2f(i, j-1));
	}

	if(j < numParticles-1){
		neighbors.push_back(Vector2f(i,j+1));
	}

	return neighbors;
}

vector<Vector2f> ClothSystem::getShear(int i, int j){
	int numParticles = int(sqrt(m_numParticles));
	vector<Vector2f> neighbors;
	if(i > 0 && j > 0){
		neighbors.push_back(Vector2f(i-1, j-1));
	}
	if(i < numParticles - 1 && j > 0){
		neighbors.push_back(Vector2f(i+1, j-1));
	}

	if(i < numParticles - 1 && j < numParticles - 1){
		neighbors.push_back(Vector2f(i+1, j+1));
	}

	if(i > 0 && j < numParticles - 1){
		neighbors.push_back(Vector2f(i-1, j+1));
	}

	return neighbors;
}

vector<Vector2f> ClothSystem::getFlex(int i, int j){
	int numParticles = int(sqrt(m_numParticles));
	vector<Vector2f> neighbors;
	if(j + 2 < numParticles){
		neighbors.push_back(Vector2f(i, j + 2));
	}

	if(i + 2 < numParticles){
		neighbors.push_back(Vector2f(i + 2, j));
	}

	if(i + 2 < numParticles && j + 2 < numParticles){
		neighbors.push_back(Vector2f(i+2, j+2));
	}

	return neighbors;
}

int ClothSystem::indexOf(int i, int j){
	float index = 2 * (i * int(sqrt(m_numParticles)) + j);
	return int(index);
}

bool ClothSystem::inFlex(int i, int j){
	for(int n = 0; n < flex_coords.size(); n++){
		if(flex_coords[n] == Vector2f(i, j)){
			return true;
		}
	}
	return false;
}

Vector3f ClothSystem::sumForces(int i, int j, vector<Vector3f> state){
	//anchored case
	int numParticles = int(sqrt(m_numParticles));
	if( (i == 0 || i == numParticles - 1) && j == 0){
		return Vector3f(0,0,0);
	}

	float mass = 5;
	int k = 4;
	float r = 0.5;
	
	//computing the gravity vector
	Vector3f Fg = mass * Vector3f(0,-1,0);
	
	//computing the drag vector
	Vector3f Fd = -1.0 * k * state[ClothSystem::indexOf(i,j) + 1];

	Vector3f pos = state[ClothSystem::indexOf(i,j)];
	Vector3f F_spring = Vector3f(0,0,0);

	vector<Vector2f> neighbors = getNeighbors(i, j);

	for(int n = 0; n < neighbors.size(); n++){
		Vector2f pair = neighbors[n];
		Vector3f other_pos = state[indexOf(pair[0], pair[1])];
		Vector2f test = pair - Vector2f(i, j);

		//Adjacent neighbors only
		if(test.abs() == 1.0){
			// if(pair[0] - i > 0){
				Vector3f d = pos - other_pos;
				float length = d.abs();
				float scalar = -1.0 * k * (length - r) / length;
				F_spring = F_spring + (scalar * d);
		}
		
	}

	neighbors = getShear(i, j);

	for(int n = 0; n < neighbors.size(); n++){
		Vector2f pair = neighbors[n];
		Vector3f other_pos = state[indexOf(pair[0], pair[1])];
		Vector3f d = pos - other_pos;
		float length = d.abs();
		float scalar = -1.0 * k * (length - r) / length;
		F_spring = F_spring + (scalar * d);
	}
	neighbors = getFlex(i, j);
	if(neighbors.size() == 3){
		//top left
		if(inFlex(i, j + 2) && inFlex(i+2, j)){
			Vector3f other_pos = state[indexOf(i, j+2)];
			Vector3f d = pos - other_pos;
			float length = d.abs();
			float scalar = -1.0 * k * (length - r) / length;
			F_spring = F_spring + (scalar * d);

			other_pos = state[indexOf(i+2, j)];
			d = pos - other_pos;
			length = d.abs();
			scalar = -1.0 * k * (length - r) / length;
			F_spring = F_spring + (scalar * d);

		}//top right
		else if(inFlex(i-2,j) && inFlex(i,j+2)){
			Vector3f other_pos = state[indexOf(i-2, j)];
			Vector3f d = pos - other_pos;
			float length = d.abs();
			float scalar = -1.0 * k * (length - r) / length;
			F_spring = F_spring + (scalar * d);

			other_pos = state[indexOf(i, j+2)];
			d = pos - other_pos;
			length = d.abs();
			scalar = -1.0 * k * (length - r) / length;
			F_spring = F_spring + (scalar * d);

		}//bottom left
		else if(inFlex(i, j-2) && inFlex(i+2,j)){
			Vector3f other_pos = state[indexOf(i, j-2)];
			Vector3f d = pos - other_pos;
			float length = d.abs();
			float scalar = -1.0 * k * (length - r) / length;
			F_spring = F_spring + (scalar * d);

			other_pos = state[indexOf(i+2, j)];
			d = pos - other_pos;
			length = d.abs();
			scalar = -1.0 * k * (length - r) / length;
			F_spring = F_spring + (scalar * d);

		}//bottom right
		else if(inFlex(i,j-2) && inFlex(i-2, j)){
			Vector3f other_pos = state[indexOf(i, j-2)];
			Vector3f d = pos - other_pos;
			float length = d.abs();
			float scalar = -1.0 * k * (length - r) / length;
			F_spring = F_spring + (scalar * d);

			other_pos = state[indexOf(i-2, j)];
			d = pos - other_pos;
			length = d.abs();
			scalar = -1.0 * k * (length - r) / length;
			F_spring = F_spring + (scalar * d);

		}
	}else if(neighbors.size() == 1){
		Vector3f other_pos = state[indexOf(neighbors[0][0], neighbors[0][1])];
		Vector3f d = pos - other_pos;
		float length = d.abs();
		float scalar = -1.0 * k * (length - r) / length;
		F_spring = F_spring + ( scalar * d);
	}
	

	return (1.0/mass) * (Fg + Fd + F_spring);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	float temp = sqrt(m_numParticles);
	int numParticles = int(temp);
	for(int i = 0; i < numParticles; i++){
		for(int j = 0; j < numParticles; j++){
			f.push_back(state[ClothSystem::indexOf(i,j) + 1]);
			f.push_back(ClothSystem::sumForces(i,j,state));
		}
	}
	return f;
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
	float temp = sqrt(m_numParticles);
	int numParticles = int(temp);
	for (int i = 0; i < numParticles; i++) {
		for(int j = 0; j < numParticles; j++){
			int index = indexOf(i, j);
			vector<Vector2f> neighbors = getNeighbors(i,j);
			Vector3f pos = m_vVecState[index];
			glPushMatrix();
			glBegin(GL_LINES);
			for(int n = 0; n < neighbors.size(); n++){
				Vector2f pair = neighbors[n];
				Vector3f other_pos = m_vVecState[indexOf(pair[0], pair[1])];
				Vector2f test = pair - Vector2f(i, j);

				if(test.abs() == 1.0){
				
					glVertex3f(pos[0], pos[1], pos[2]);
					glVertex3f(other_pos[0], other_pos[1], other_pos[2]);
				}
				
			}
			glEnd();
			glTranslatef(pos[0], pos[1], pos[2]);
			glutSolidSphere(0.075f,10.0f,10.0f);
			glPopMatrix();
		}
	}
}