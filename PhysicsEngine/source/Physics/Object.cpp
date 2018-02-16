#include "Physics\Object.h"
#include "Physics\Sphere.h"
#include "Gizmos.h"
#include <glm/geometric.hpp>
using namespace Physics;
using glm::vec3;

Physics::Object::Object(ShapeType shape, vec3 pos, float mass, vec4 color, bool isStatic) :
	m_shape (shape), m_position(pos), m_mass(mass), m_color(color), m_isStatic(isStatic)
{
	m_velocity = vec3();
	m_acceleration = vec3();
}

Object::~Object()
{
}

bool Physics::Object::isColliding(Object * other)
{
	switch (m_shape)
	{
	case SPHERE:
	{
		switch (other->getShapeType())
		{
		case SPHERE:
		{
			return isCollidingSphereSphere((Sphere *)this, (Sphere *)other);
		}
		break;
		return false;
		}
	}
	break;
	case AABB:
	{}
	break;
	case PLANE:
	{}
	break;
	return false;
	}
	return false;
}

bool Physics::Object::isCollidingSphereSphere(Sphere * objA, Sphere * objB)
{
	// Assumes both are spheres
	assert(objA != nullptr);
	assert(objB != nullptr);

	// Find distance between centers
	float distance = glm::distance(objB->getPosition(), objA->getPosition());

	// Add up the two radii
	float radii = objA->getRadius() + objB->getRadius();

	// Is the distance smaller than the radii
	return distance < radii;
}

void Object::update(float deltaTime)
{
	if (!m_isStatic)
	{
		// Apply friction (dampening)
		applyForce(-m_velocity * m_friction);

		m_velocity += m_acceleration * deltaTime;
		m_position += m_velocity * deltaTime;

		m_acceleration = vec3(); // Reset acceleration to zero
	}
}

void Object::applyForce(const vec3 & force)
{
	// Force = Mass * Acceleration
	m_acceleration += force / m_mass;
}

void Physics::Object::applyImpulse(const vec3 & impulse)
{
	m_velocity += impulse;
}
