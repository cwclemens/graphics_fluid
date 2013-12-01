#ifndef KERNEL_H
#define KERNEL_H

#include <vector>
#include <vecmath.h>

using namespace std;

class Kernel {
public:
	Kernel(ParticleSystem* ps){
		this->ps = ps;
	};

	
protected:
	ParticleSystem* ps;
};

#endif