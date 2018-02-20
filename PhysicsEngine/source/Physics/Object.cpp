#include "Physics/Object.h"
#include "Physics/Sphere.h"
#include "Physics/Plane.h"
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

bool Physics::Object::isColliding(Object * other, vec3 & collisionNormal)
{
	switch (m_shape)
	{
	case SPHERE:
	{
		switch (other->getShapeType())
		{
		case SPHERE:
		{
			return isCollidingSphereSphere((Sphere *)this, (Sphere *)other, collisionNormal);
		}
		case PLANE:
		{
			return isCollidingPlaneSphere((Plane *)other, (Sphere*)this, collisionNormal);
		}
		}
	}
	case AABB:
	{}
	case PLANE:
	{
		switch (other->getShapeType())
		{
		case SPHERE:
		{
			return isCollidingPlaneSphere((Plane *)this, (Sphere *)other, collisionNormal);
		}
		}
	}
	}
	return false;
}

bool Physics::Object::isCollidingSphereSphere(Sphere * objA, Sphere * objB, vec3 &collisionNormal)
{
	// Assumes both are spheres
	assert(objA != nullptr);
	assert(objB != nullptr);

	// Find distance between centers
	float distance = glm::distance(objB->getPosition(), objA->getPosition());

	// Add up the two radii
	float radii = objA->getRadius() + objB->getRadius();

	// Is the distance smaller than the radii
	if (distance < radii)
	{
		collisionNormal = glm::normalize(objB->getPosition() - objA->getPosition());
		return true;
	}
	return false;

}

bool Physics::Object::isCollidingPlaneSphere(Plane * objA, Sphere * objB, vec3 &collisionNormal)
{
	vec3 spherePosition = objA->getPosition() + objB->getPosition();

	float distance = glm::dot(spherePosition, objA->getDirection()) - objA->getDistance();
	if (distance < objB->getRadius())
	{
		collisionNormal = objA->getDirection();
		return true;
	}
	return false;
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
