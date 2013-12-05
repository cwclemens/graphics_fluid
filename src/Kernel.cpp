
#include <vector>
#include <vecmath.h>
#include <cmath>
#include "extra.h"
#include "FluidParticle.h"
#include "Kernel.h"
#include "FluidSystem.h"


using namespace std;

//////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

Kernel::Kernel(FluidSystem* fs){
	this->fs = fs;
	this->h = 1.0f;
	h2 = h*h;
	h3 = h2*h;
	h6 = h3*h3;
	h9 = h3*h6;
	ph2 = M_PI*h2;
	ph3 = M_PI*h3;
	ph6 = M_PI*h6;
	ph9 = M_PI*h9;
}

Kernel::Kernel(FluidSystem* fs, float h) {
	this->fs = fs;
	this->h = h;
	h2 = h*h;
	h3 = h2*h;
	h6 = h3*h3;
	h9 = h3*h6;
	ph2 = M_PI*h2;
	ph3 = M_PI*h3;
	ph6 = M_PI*h6;
	ph9 = M_PI*h9;
}

void Kernel::updateDensities(){
	float k = this->fs->stiffness;
	float rho_o = this->fs->restDensity;
	for(int i = 0; i < this->fs->particles.size(); i++){
		float rho = computeMassDensity(i);
		this->fs->particles[i].pressure = rho;
		this->fs->particles[i].pressure = k*(rho - rho_o);
	}
}

vector<FluidParticle> Kernel::computeNeighbors(int index){
	FluidParticle p = this->fs->particles[index];
	vector<FluidParticle> neighbors;
	for(int i = 0; i < this->fs->particles.size(); i++){
		if(i != index){
			FluidParticle j = this->fs->particles[i];
			if ((p.position - j.position).absSquared() < h2){
				neighbors.push_back(j);
			}
		}
	}

	return neighbors;
}

float Kernel::computeMassDensity(int index){
	float rho = 0.0f;
	float mass = this->fs->mass;
	FluidParticle p = this->fs->particles[index];
	for(int i = 0; i < computeNeighbors(index).size(); i++){
		if(i != index){
			rho += mass * defaultWeight(p.position - this->fs->particles[i].position);
		}
	}
	return rho;
}

Vector3f Kernel::computeInternalPressure(int index){
	Vector3f pressure;
	FluidParticle p = this->fs->particles[index];
	float pi = p.pressure;
	float rho = p.density;
	float mass = this->fs->mass;
	for(int i = 0; i < computeNeighbors(index).size(); i++){
		FluidParticle j = this->fs->particles[index];
		float pj = j.pressure;
		float rho_j = j.density;
		pressure += ((pi / pow(rho, 2)) + (pj / pow(rho_j, 2))) * mass * 
			gradPressureWeight(p.position - j.position);
	}
	pressure = - rho * pressure;
	return pressure;
}

Vector3f Kernel::computeViscosity(int index){
	Vector3f viscosity;
	FluidParticle p = this->fs->particles[index];
	Vector3f ui = state[2*index + 1];
	float mass = this->fs->mass;
	float mu = this->fs->viscosity;
	for(int i = 0; i < computeNeighbors(i).size(); i++){
			FluidParticle j = this->fs->particles[i];
			viscosity = (j.velocity - p.velocity) * (mass / j.density) 
				* laplaceViscosityWeight(p.position - j.position);
	}
	viscosity = mu * viscosity;
	return viscosity;
}

Vector3f Kernel::computeInternalForces(int index){
	return computeInternalPressure(index) + computeViscosity(index);
}





/////////////////////////
// PROTECTED FUNCTIONS //
/////////////////////////

float Kernel::gaussianWeight(Vector3f r, float h) {
	float h2 = h*h;
	return exp(-r.absSquared()/(2*h2))/pow(2*M_PI*h2, 1.5);
}

float Kernel::gaussianWeight(Vector3f r) {
	return exp(-r.absSquared()/(2*h2))/pow(2*ph2, 1.5);
}

float Kernel::defaultWeight(Vector3f r, float h) {
	float rmag = r.abs();
	if (rmag > h) return 0;
	return 315*pow(h*h - rmag*rmag, 3)/(64*M_PI*pow(h, 9));
}

float Kernel::defaultWeight(Vector3f r) {
	float rmag = r.abs();
	if (rmag > h) return 0;
	return 315*pow(h2 - rmag*rmag, 3)/(64*ph9);
}

Vector3f Kernel::gradDefaultWeight(Vector3f r, float h) {
	return -945*r*pow(h*h - r.absSquared(), 2)/(32*M_PI*pow(h, 9));
}

Vector3f Kernel::gradDefaultWeight(Vector3f r) {
	return -945*r*pow(h2 - r.absSquared(), 2)/(32*ph9);
}

float Kernel::laplaceDefaultWeight(Vector3f r, float h) {
	float h2 = h*h;
	float r2 = r.absSquared();
	return -945*(h2 - r2)*(3*h2 - 7*r2)/(32*M_PI*pow(h, 9));
}

float Kernel::laplaceDefaultWeight(Vector3f r) {
	float r2 = r.absSquared();
	return -945*(h2 - r2)*(3*h2 - 7*r2)/(32*ph9);
}

float Kernel::pressureWeight(Vector3f r, float h) {
	float rmag = r.abs();
	if (rmag > h) return 0;
	return 15*pow(h - rmag, 3)/(M_PI*pow(h, 6));
}

float Kernel::pressureWeight(Vector3f r) {
	float rmag = r.abs();
	if (rmag > h) return 0;
	return 15*pow(h - rmag, 3)/ph6;
}

Vector3f Kernel::gradPressureWeight(Vector3f r, float h) {
	return -45*r.normalized()*pow(h - r.abs(), 2)/(M_PI*pow(h, 6));
}

Vector3f Kernel::gradPressureWeight(Vector3f r) {
	return -45*r.normalized()*pow(h - r.abs(), 2)/ph6;
}

float Kernel::laplacePressureWeight(Vector3f r, float h) {
	float rmag = r.abs();
	return -90*(h - rmag)*(h - 2*rmag)/(M_PI*pow(h, 6)*rmag);
}

float Kernel::laplacePressureWeight(Vector3f r) {
	float rmag = r.abs();
	return -90*(h - rmag)*(h - 2*rmag)/(ph6*rmag);
}

float Kernel::viscosityWeight(Vector3f r, float h) {
	float rmag = r.abs();
	if (rmag > h) return 0;
	return 15*(-rmag*rmag*rmag/(2*h*h*h) + rmag*rmag/(h*h) + h/(2*rmag) + 1)/(2*M_PI*h*h*h);
}

float Kernel::viscosityWeight(Vector3f r) {
	float rmag = r.abs();
	if (rmag > h) return 0;
	return 15*(-rmag*rmag*rmag/(2*h3) + rmag*rmag/h2 + h/(2*rmag) + 1)/(2*ph3);
}

Vector3f Kernel::gradViscosityWeight(Vector3f r, float h) {
	float rmag = r.abs();
	return 15*r*(-3*rmag/(2*h*h*h) + 2/(h*h) + - h/(2*rmag*rmag*rmag))/(2*M_PI*h*h*h);
}

Vector3f Kernel::gradViscosityWeight(Vector3f r) {
	float rmag = r.abs();
	return 15*r*(-3*rmag/(2*h3) + 2/h2 + - h/(2*rmag*rmag*rmag))/(2*ph3);
}

float Kernel::laplaceViscosityWeight(Vector3f r, float h) {
	return 45*(h-r.abs())/(M_PI*pow(h, 6));
}

float Kernel::laplaceViscosityWeight(Vector3f r) {
	return 45*(h-r.abs())/ph6;
}

