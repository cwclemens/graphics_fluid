#include "TimeStepper.h"


inline vector<Vector3f> mul(float c, vector<Vector3f> v)
{
	for(unsigned i=0; i<v.size(); i++) {
		v[i] = c * v[i];
	}
	return v;
}

inline vector<Vector3f> add(vector<Vector3f> a, vector<Vector3f> b)
{
	for(unsigned i=0; i<a.size(); i++) {
		a[i] = a[i] + b[i];
	}
	return a;
}

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> state = particleSystem->getState();
	particleSystem->setState(add(state, mul(stepSize, particleSystem->evalF(state))));
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> state = particleSystem->getState();
	vector<Vector3f> f0 = particleSystem->evalF(state);
	vector<Vector3f> f1 = particleSystem->evalF(add(state, mul(stepSize, f0)));
	particleSystem->setState(add(state, mul(0.5*stepSize, add(f0, f1))));
}

void RK4::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> state = particleSystem->getState();
	vector<Vector3f> k1 = particleSystem->evalF(state);
	vector<Vector3f> k2 = particleSystem->evalF(add(state, mul(stepSize/2.0f, k1)));
	vector<Vector3f> k3 = particleSystem->evalF(add(state, mul(stepSize/2.0f, k2)));
	vector<Vector3f> k4 = particleSystem->evalF(add(state, mul(stepSize, k3)));
	particleSystem->setState(add(state, mul(stepSize/6.0f, add(k1, add(mul(2, k2), add(mul(2, k3), k4))))));
}
