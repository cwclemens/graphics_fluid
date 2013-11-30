#ifndef FLUIDPARTICLE_H
#define FLUIDPARTICLE_H

#include <vector>
#include <vecmath.h>

using namespace std;

class FluidParticle 
{
public:
	FluidParticle(Vector3f pos, Vector3f vel, float d){
		position = pos;
		velocity = vel;
		density = d;
		pressure = 0.0f;
	};

	FluidParticle(){
		position = Vector3f();
		velocity = Vector3f();
		density = 1.0f;
		pressure = 1.0f;
	}

	void draw(){
		//TODO: Render a single particle
	}

	Vector3f position;
	Vector3f velocity;
	float density;
	float pressure;
};

#endif