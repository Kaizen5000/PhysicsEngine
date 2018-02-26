#include "Physics/Object.h"
#include "Physics/Sphere.h"
#include "Physics/Plane.h"
#include "Gizmos.h"
#include <glm/geometric.hpp>
using namespace Physics;
using glm::vec3;

// Constructor
Physics::Object::Object(ShapeType shape, vec3 pos, float mass, vec4 color, bool isStatic) :
	m_shape (shape), m_position(pos), m_mass(mass), m_color(color), m_isStatic(isStatic)
{
	// Sets the velocity and acceleration to 0 on initialisation
	m_velocity = vec3();
	m_acceleration = vec3();
}

// Destructor
Object::~Object()
{
}

// This uses case statements to identify this object and the other object
// Then it delete
bool Physics::Object::isColliding(Object * other, vec3 & collisionNormal)
{
	// Case statement to identify this shape
	switch (m_shape)
	{
	case SPHERE:
	{
		// After identifying this shape is a sphere, another case statement is used to identify the other shape
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
		// After identifying this shape is a plane, another case statement is used to identify the other shape
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
	// Checks that both object pointers are not null
	assert(objA != nullptr);
	assert(objB != nullptr);

	// Find distance between centers
	float distance = glm::distance(objB->getPosition(), objA->getPosition());

	// Add up the two radii
	float radii = objA->getRadius() + objB->getRadius();

	// Checks if the distance is less than the radii
	if (distance < radii)
	{
		// Set collision normal
		collisionNormal = glm::normalize(objB->getPosition() - objA->getPosition());
		// Return true as there is a collision
		return true;
	}

	// If the distance is not less than the radii, there is no collision
	return false;

}

bool Physics::Object::isCollidingPlaneSphere(Plane * objA, Sphere * objB, vec3 &collisionNormal)
{
	// Get the sphere position, calculated as if the plane was at 0,0,0
	vec3 spherePosition = objA->getPosition() + objB->getPosition();
	
	// The distance is the dot product of the spherePosition and plane normal, minus the plane distance
	// This projects the sphere distance onto the closest point on the plane
	float distance = glm::dot(spherePosition, objA->getDirection()) - objA->getDistance();

	// If the distance is less than the radius of the sphere, there is a collision
	if (distance < objB->getRadius())
	{
		// Assigns the collision normal, which for plane - sphere collison is always the plane normal
		collisionNormal = objA->getDirection();

		// Seperate the sphere from the plane if they are overlapping
		objB->setPosition(objB->getPosition() + collisionNormal * (objB->getRadius() -  distance));
		return true;
	}
	return false;
}

void Object::update(float deltaTime)
{
	// If the object is not static, update
	if (!m_isStatic)
	{
		// Apply friction (dampening)
		applyForce(-m_velocity * m_friction);

		// Increase velocity by acceleration times delta time
		m_velocity += m_acceleration * deltaTime;

		// Moves the object's position by velocity times delta time
		m_position += m_velocity * deltaTime;

		// Reset acceleration to zero
		m_acceleration = vec3(); 
	}
}

void Object::applyForce(const vec3 & force)
{
	// Increases acceleration by force divided my mass
	// Force = Mass * Acceleration
	m_acceleration += force / m_mass; 
}

void Physics::Object::applyImpulse(const vec3 & impulse)
{
	// Adds impulse to velocity, not altered by delta time
	m_velocity += impulse;
}
