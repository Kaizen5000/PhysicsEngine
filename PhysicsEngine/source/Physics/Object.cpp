#include "Physics/Object.h"
#include "Physics/Sphere.h"
#include "Physics/Plane.h"
#include "Physics/AABB.h"
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
bool Physics::Object::isColliding(Object * other, vec3 & collisionNormal)
{
	// Case statement to identify this shape
	switch (m_shape)
	{
	case ShapeType::SPHERE:
	{
		// After identifying this shape is a sphere, another case statement is used to identify the other shape
		switch (other->getShapeType())
		{
		case ShapeType::SPHERE:
		{
			return isCollidingSphereSphere((Sphere*)this, (Sphere*)other, collisionNormal);
		}
		case ShapeType::PLANE:
		{
			return isCollidingPlaneSphere((Plane*)other, (Sphere*)this, collisionNormal);
		}
		case ShapeType::AABB:
		{
			return isCollidingSphereAABB((Sphere*)this, (AABB*)other, collisionNormal);
		}
		}
	}
	case ShapeType::AABB:
	{
		switch (other->getShapeType())
		{
		case ShapeType::PLANE:
		{
			return isCollidingPlaneAABB((Plane*)other, (AABB*)this, collisionNormal);
		}
		case ShapeType::SPHERE:
		{
			return isCollidingSphereAABB((Sphere*)other, (AABB*)this, collisionNormal);
		}
		case ShapeType::AABB:
		{
			return isCollidingAABBAABB((AABB*)this, (AABB*)other, collisionNormal);
		}
		}
	}
	case ShapeType::PLANE:
	{
		// After identifying this shape is a plane, another case statement is used to identify the other shape
		switch (other->getShapeType())
		{
		case ShapeType::SPHERE:
		{
			return isCollidingPlaneSphere((Plane*)this, (Sphere*)other, collisionNormal);
		}
		case ShapeType::AABB:
		{
			return isCollidingPlaneAABB((Plane*)this, (AABB*)other, collisionNormal);
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
	// The distance is the dot product of the spherePosition and plane normal, minus the plane distance
	// This projects the sphere distance onto the closest point on the plane
	float distance = glm::dot(objB->getPosition(), objA->getDirection()) - objA->getDistance();

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

bool Physics::Object::isCollidingPlaneAABB(Plane * objA, AABB * objB, vec3 & collisionNormal)
{
	float distance = glm::dot(objB->getPosition(), objA->getDirection()) - objA->getDistance();

	float radius =	glm::dot(objB->getExtents().x, objA->getDirection().x) +
					glm::dot(objB->getExtents().y, objA->getDirection().y) +
					glm::dot(objB->getExtents().z, objA->getDirection().z);
	
	if (distance > objB->getExtents().x || distance > objB->getExtents().y || distance > objB->getExtents().z)
	{
		return false;
	}
	else
	{
		
		collisionNormal = objA->getDirection();

		objB->setPosition(objB->getPosition() + collisionNormal * (radius - distance));
		return true;
	}
	return false;
}

bool Physics::Object::isCollidingAABBAABB(AABB * objA, AABB * objB, vec3 & collisionNormal)
{
	// Displacement
	vec3 distance = objB->getPosition() - objA->getPosition();
	// Get the absolute value of the distance to account for objB being behind objA
	vec3 absDistance = glm::abs(distance);

	vec3 totalExtents = objA->getExtents() + objB->getExtents();

	if (absDistance.x > totalExtents.x || absDistance.y > totalExtents.y || absDistance.z > totalExtents.z)
	{	
		// No collision
		return false;
	}

	glm::vec3 overlap = totalExtents - absDistance;
	float smallestOverlap = glm::min(glm::min(overlap.x, overlap.y), overlap.z);
	if (smallestOverlap == overlap.x)
	{
		collisionNormal = glm::vec3(1, 0, 0) * glm::sign(distance.x);
	}
	else if (smallestOverlap == overlap.y)
	{
		collisionNormal = glm::vec3(0, 1, 0) * glm::sign(distance.y);
	}
	else  // z
	{
		collisionNormal = glm::vec3(0, 0, 1) * glm::sign(distance.z);
	}
	return true;
}

bool Physics::Object::isCollidingSphereAABB(Sphere * objA, AABB * objB, vec3 & collisionNormal)
{
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
