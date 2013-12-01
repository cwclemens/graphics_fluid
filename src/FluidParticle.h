#ifndef FLUIDPARTICLE_H
#define FLUIDPARTICLE_H

#include <vector>
#include <vecmath.h>
#include "extra.h"

using namespace std;

class FluidParticle 
{
public:
	FluidParticle(Vector3f pos, Vector3f vel){
		position = pos;
		velocity = vel;
		density = 0.0f;
		pressure = 0.0f;
	};

	FluidParticle(){
		position = Vector3f();
		velocity = Vector3f();
		density = 1.0f;
		pressure = 1.0f;
	}

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
