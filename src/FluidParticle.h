#ifndef FLUIDPARTICLE_H
#define FLUIDPARTICLE_H

#include <vector>
#include <vecmath.h>
#include "extra.h"
#include <iostream>

using namespace std;

class FluidParticle 
{
public:
	FluidParticle(Vector3f _initialPosition, Vector3f _velocity){
		density = 0.0f;
		pressure = 0.0f;
		position = _initialPosition;
		velocity = _velocity;
	};

	void draw(){
		glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}

	Vector3f position;
	Vector3f velocity;
	float density;
	float pressure;
};

#endif
