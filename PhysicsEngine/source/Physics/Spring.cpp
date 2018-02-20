#include "Physics/Spring.h"
#include "Physics/Object.h"
#include <Gizmos.h>
using namespace Physics;

Physics::Spring::Spring(Object * objA, Object * objB, float restingLength, float springCoefficient, float damping) : 
	m_restingLength(restingLength), m_springCoefficient(springCoefficient), m_damping(damping),Constraint( objA, objB)
{
}

Spring::~Spring()
{
}

void Physics::Spring::update(float deltaTime)
{
	vec3 springVec = m_objA->getPosition() - m_objB->getPosition();
	float distance = glm::length(springVec);
	vec3 force = vec3();
	if (distance != 0.f)
	{
		force += -(springVec / distance) * (distance - m_restingLength) * m_springCoefficient;
	}
	force +=  -(m_objA->getVelocity() - m_objB->getVelocity()) * m_damping;
	m_objA->applyForce(force);
	m_objB->applyForce(-force);
}

void Physics::Spring::draw()
{
	// Draw a line to represent the spring
	aie::Gizmos::addLine(m_objA->getPosition(), m_objB->getPosition(), vec4(1.f, 1.f, 1.f, 1.f));
}
