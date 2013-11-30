#include "TimeStepper.h"

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> state = particleSystem->getState();
	vector<Vector3f> f = particleSystem->evalF(state);
	vector<Vector3f> result;
	for(int i = 0; i < f.size(); i++){
		result.push_back(state[i] + stepSize * f[i]);
	}
	particleSystem->setState(result);
}

///TODO: implement Trapezoidal rule here
void Trapezoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> state = particleSystem->getState();
	vector<Vector3f> f0 = particleSystem->evalF(state);
	vector<Vector3f> temp;
	for(int i = 0; i < f0.size(); i++){
		temp.push_back(state[i] + stepSize * f0[i]);
	}
	vector<Vector3f> f1 = particleSystem->evalF(temp);
	vector<Vector3f> result;
	for(int i = 0; i < state.size(); i++){
		result.push_back(state[i] + (stepSize/2) * (f0[i] + f1[i]));
	}
	particleSystem->setState(result);
}