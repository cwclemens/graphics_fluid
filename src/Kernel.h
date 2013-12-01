#ifndef KERNEL_H
#define KERNEL_H

#include <vector>
#include <vecmath.h>
#include <cmath>

#include "extra.h"

using namespace std;

class Kernel {
public:
	Kernel(ParticleSystem* ps){
		this->ps = ps;
		this->h = 1.0f;
		h2 = h*h;
		h3 = h2*h;
		h6 = h3*h3;
		h9 = h3*h6;
		ph3 = M_PI*h3;
		ph6 = M_PI*h6;
		ph9 = M_PI*h9;
	};

	Kernel(ParticleSystem* ps, float h) {
		this->h = h;
		h2 = h*h;
		h3 = h2*h;
		h6 = h3*h3;
		h9 = h3*h6;
		ph3 = M_PI*h3;
		ph6 = M_PI*h6;
		ph9 = M_PI*h9;
	};

	void updateDensities(){
		float k = this->ps->stiffness;
		float rho_o = this->ps->restDensity;
		for(int i = 0; i < this->ps->particles.size(); i++){
			float rho = computeMassDensity(i);
			this->ps->particles[i].pressure = rho;
			this->ps->particles[i].pressure = k*(rho - rho_o);
		}
	};

	vector<FluidParticle> computeNeighbors(int index){
		FluidParticle p = this->ps->particles[index];
		vector<FluidParticle> neighbors;
		for(int i = 0; i < this->ps->particles.size(); i++){
			if(i != index){
				FluidParticle j = this->ps->particles[i];
				if ((p.location - j.location).absSquared() < h2){
					neighbors.push_back(j);
				}
			}
		}

		return neighbors;
	};

	float computeMassDensity(int index){
		float rho = 0.0f;
		float mass = this->ps->mass;
		FluidParticle p = this->ps->particles[index];
		for(int i = 0; i < computeNeighbors(index).size(); i++){
			if(i != index){
				rho += mass * defaultWeight(p.location - this->ps->particles[i].location);
			}
		}
		return rho;
	};

	Vector3f computeInternalPressure(int index){
		Vector3f pressure;
		FluidParticle p = this->ps->particles[index];
		float pi = p.pressure;
		float rho = p.density;
		float mass = this->ps->mass;
		for(int i = 0; i < computeNeighbors(index).size(); i++){
			FluidParticle j = this->ps->particles[index];
			float pj = j.pressure;
			float rho_j = j.density;
			pressure += ((pi / pow(rho, 2)) + (pj / pow(rho_j, 2))) * mass * 
				gradPressureWeight(p.location - j.location);
		}
		pressure = - rho * pressure;
		return pressure;
	};

	Vector3f computeViscosity(int index){
		Vector3f viscosity;
		FluidParticle p = this->ps->particles[index];
		Vector3f ui = state[2*index + 1];
		float mass = this->ps->mass;
		float mu = this->ps->viscosity;
		for(int i = 0; i < computeNeighbors(i).size(); i++){
				FluidParticle j = this->ps->particles[i];
				viscosity = (j.velocity - p.velocity) * (mass / j.density) 
					* laplaceViscosityWeight(p.location - j.location);
		}
		viscosity = mu * viscosity;
		return viscosity;
	};

	Vector3f computeInternalForces(int index){
		return computeInternalPressure(index) + computeViscosity(index);
	};

	
protected:
	ParticleSystem* ps;

	float gaussianWeight(Vector3f r, float h) {
		float h2 = h*h;
		return exp(-r.absSquared()/(2*h2))/pow(2*M_PI*h2, 1.5f);
	}

	float gaussianWeight(Vector3f r) {
		return exp(-r.absSquared()/(2*h2))/pow(2*ph2, 1.5f);
	}
	
	float defaultWeight(Vector3f r, float h) {
		float rmag = r.abs();
		if (rmag > h) return 0;
		return 315*pow(h*h - rmag*rmag, 3)/(64*M_PI*pow(h, 9));
	}

	float defaultWeight(Vector3f r) {
		float rmag = r.abs();
		if (rmag > h) return 0;
		return 315*pow(h2 - rmag*rmag, 3)/(64*ph9);
	}

	Vector3f gradDefaultWeight(Vector3f r, float h) {
		return -945*r*pow(h*h - r.absSquared(), 2)/(32*M_PI*pow(h, 9));
	}

	Vector3f gradDefaultWeight(Vector3f r) {
		return -945*r*pow(h2 - r.absSquared(), 2)/(32*ph9);
	}

	float laplaceDefaultWeight(Vector3f r, float h) {
		float h2 = h*h;
		float r2 = r.absSquared();
		return -945*(h2 - r2)*(3*h2 - 7*r2)/(32*M_PI*pow(h, 9));
	}

	float laplaceDefaultWeight(Vector3f r) {
		float r2 = r.absSquared();
		return -945*(h2 - r2)*(3*h2 - 7*r2)/(32*ph9);
	}

	float pressureWeight(Vector3f r, float h) {
		float rmag = r.abs();
		if (rmag > h) return 0;
		return 15*pow(h - rmag, 3)/(M_PI*pow(h, 6));
	}

	float pressureWeight(Vector3f r) {
		float rmag = r.abs();
		if (rmag > h) return 0;
		return 15*pow(h - rmag, 3)/ph6;
	}

	Vector3f gradPressureWeight(Vector3f r, float h) {
		return -45*r.normalized()*pow(h - r.abs(), 2)/(M_PI*pow(h, 6));
	}

	Vector3f gradPressureWeight(Vector3f r) {
		return -45*r.normalized()*pow(h - r.abs(), 2)/ph6;
	}

	float laplacePressureWeight(Vector3f r, float h) {
		float rmag = r.abs();
		return -90*(h - rmag)*(h - 2*rmag)/(M_PI*pow(h, 6)*rmag);
	}

	float laplacePressureWeight(Vector3f r) {
		float rmag = r.abs();
		return -90*(h - rmag)*(h - 2*rmag)/(ph6*rmag);
	}

	float viscosityWeight(Vector3f r, float h) {
		float rmag = r.abs();
		if (rmag > h) return 0;
		return 15*(-rmag*rmag*rmag/(2*h*h*h) + rmag*rmag/(h*h) + h/(2*rmag) + 1)/(2*M_PI*h*h*h);
	}

	float viscosityWeight(Vector3f r) {
		float rmag = r.abs();
		if (rmag > h) return 0;
		return 15*(-rmag*rmag*rmag/(2*h3) + rmag*rmag/h2 + h/(2*rmag) + 1)/(2*ph3);
	}

	Vector3f gradViscosityWeight(Vector3f r, float h) {
		float rmag = r.abs();
		return 15*r*(-3*rmag/(2*h*h*h) + 2/(h*h) + - h/(2*rmag*rmag*rmag))/(2*M_PI*h*h*h);
	}

	Vector3f gradViscosityWeight(Vector3f r) {
		float rmag = r.abs();
		return 15*r*(-3*rmag/(2*h3) + 2/h2 + - h/(2*rmag*rmag*rmag))/(2*ph3);
	}

	float laplaceViscosityWeight(Vector3f r, float h) {
		return 45*(h-r.abs())/(M_PI*pow(h, 6));
	}

	float laplaceViscosityWeight(Vector3f r) {
		return 45*(h-r.abs())/ph6;
	}

	float h, h2, h3, h6, h9, ph3, ph6, ph9;
};

#end
