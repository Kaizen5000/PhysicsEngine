#include "Physics/Spring.h"
using namespace Physics;

Physics::Spring::Spring(Object * objA, Object * objB, float restingLength, float springCoefficient, float damping) : Constraint( objA, objB)
{
}

Spring::~Spring()
{
}

void Physics::Spring::update(float deltaTime)
{

}

void Physics::Spring::draw()
{
}
