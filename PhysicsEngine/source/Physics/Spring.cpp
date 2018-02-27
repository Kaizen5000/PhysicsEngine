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
	// A vector for the between the two objects that the spring connects
	vec3 springVec = m_objA->getPosition() - m_objB->getPosition();
	
	// The float distance between the two objects
	float distance = glm::length(springVec);

	// Force vector, starting at 0,0,0
	vec3 force = vec3();

	// If the objects are not directly overlapping
	if (distance != 0.f)
	{
		// Increase force
		// -vector normal * how far the distance is from resting length * strength of spring
		force += -(springVec / distance) * (distance - m_restingLength) * m_springCoefficient;
	}

	// Apply dampening
	force +=  -(m_objA->getVelocity() - m_objB->getVelocity()) * m_damping;

	// Apply the force to the objects
	m_objA->applyForce(force);
	m_objB->applyForce(-force);
}

void Physics::Spring::draw()
{
	// Draw a line to represent the spring
	aie::Gizmos::addLine(m_objA->getPosition(), m_objB->getPosition(), vec4(1.f, 1.f, 1.f, 1.f));
}
