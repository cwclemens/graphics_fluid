#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "extra.h"
#include <vector>

#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int numParticles);
	int indexOf(int i, int j);
	vector<Vector2f> getNeighbors(int i, int j);
	vector<Vector2f> getShear(int i, int j);
	vector<Vector2f> getFlex(int i, int j);
	bool inFlex(int i, int j);
	Vector3f sumForces(int i, int j, vector<Vector3f> state);
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

private:

};


#endif
