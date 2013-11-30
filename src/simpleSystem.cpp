
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
	m_vVecState.push_back(Vector3f(1,0,0));
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	float x = state[0][0], y = state[0][1], z = state[0][2]; 
	f.push_back(Vector3f(-1*y,x,0));

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
	Vector3f pos = m_vVecState[0];
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2] );
	glutSolidSphere(0.075f,10.0f,10.0f);
	glPopMatrix();
}
