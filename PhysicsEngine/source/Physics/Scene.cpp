#include "Physics/Scene.h"
#include "Physics/Object.h"
#include "Physics/Sphere.h"
#include "Physics/Plane.h"
#include "Physics/Spring.h"
#include <Gizmos.h>

using namespace Physics;
using glm::vec4;

Scene::Scene()
{
	// Default gravity just in case
	m_gravity = vec3(0.0f, -9.8f, 0.0f);

	//Defaults for fixed time at 100fps
	m_fixedTimeStep = 0.01f;

	// Set accumulated time to 0
	m_accumulatedTime = 0.0f;

	// Zero the global force
	m_globalForce = vec3();
}


Scene::~Scene()
{
	// Delete all springs
	for (auto spring : m_springs)
	{
		delete spring;
	}

	// Delete all objects
	for (auto object : m_objects)
	{
		delete object;
	}
}

void Scene::update(float deltaTime)
{
	// Increase accumulated time by delta time
	m_accumulatedTime += deltaTime;

	// Each iteration uses m_fixedTimeStep as delta time
	// The loop continues until the sum of fixed time steps is equal to or less than m_accumulated time
	while (m_accumulatedTime >= m_fixedTimeStep)
	{
		// Applies gravity to all objects
		applyGravity();

		// Updates all objects with fixed time step
		for (auto object : m_objects)
		{
			object->update(m_fixedTimeStep);
		}

		// Updated all springs with fixed time step
		for (auto spring : m_springs)
		{
			spring->update(m_fixedTimeStep);
		}
		
		// Decrement the accumulated time
		m_accumulatedTime -= m_fixedTimeStep;

		// Check for collisions
		checkCollision();

		// Resolve collisions
		resolveCollision();
	}
}

void Scene::draw()
{
	// Draws all objects
	for (auto object : m_objects)
	{
		object->draw();
	}

	// Draws all springs
	for (auto spring : m_springs)
	{
		spring->draw();
	}
}

void Scene::addObject(Object * object)
{
	// Adds the parameter object to the vector
	m_objects.push_back(object);
}

void Scene::removeObject(Object * object)
{
	// Find object in vector
	auto iter = std::find(m_objects.begin(), m_objects.end(), object);
	// If found remove from vector
	if (iter != m_objects.end())
	{
		m_objects.erase(iter);
	}
}

void Physics::Scene::addSpring(Spring * spring)
{
	// Adds the spring to the vector
	m_springs.push_back(spring);
}

void Physics::Scene::removeSpring(Spring * spring)
{
	// Find object in vector
	auto iter = std::find(m_springs.begin(), m_springs.end(), spring);
	// If found, remove from vector
	if (iter != m_springs.end())
	{
		m_springs.erase(iter);
	}
}

void Scene::applyGlobalForce()
{
	// Applies global force to all objects
	for (auto object : m_objects)
	{
		object->applyForce(m_globalForce);
	}
}

void Scene::applyGravity()
{
	// Applies gravity to all objects
	for (auto object : m_objects)
	{
		// Since gravity applies force based on mass
		object->applyForce(m_gravity* object->getMass());
	}
}

void Physics::Scene::checkCollision()
{
	// Loops through all objects to find collisions, then place them in the collision vector
	for (auto object  = m_objects.begin(); object != m_objects.end(); object++)
	{
		// Loops through objects that the first object can collide with, the nature of this loop is that it checks
		// against objects forward in the vector
		for (auto object2 = object + 1; object2 != m_objects.end(); object2++)
		{
			Collision tempCollision;
			// Passes both objects and a reference to the collision normal of tempCollision into the collision check function
			// Uses the return bool and adds to collision vector if there is a collision
			if ((*object)->isColliding(*object2, tempCollision.collisionNormal))
			{
				// For the specific case where the first object is a sphere and the second object is a plane, they are added to the struct in reverse order
				if ((*object)->getShapeType() == ShapeType::SPHERE && (*object2)->getShapeType() == ShapeType::PLANE)
				{
					tempCollision.objA = *object2;
					tempCollision.objB = *object;
				}
				else
				{
					tempCollision.objA = *object;
					tempCollision.objB = *object2;
				}
				// Adds the struct to the vector
				m_collisions.push_back(tempCollision);
			}
		}
	}
}

void Physics::Scene:: resolveCollision()
{

	for (auto col : m_collisions)
	{
		// If both objects are static, skip collision resolution
		if (col.objA->getIsStatic() && col.objB->getIsStatic()) continue;
	
		// Inverse masses
		float inverseMassObjA = 1 / col.objA->getMass();
		float inverseMassObjB = 1 / col.objB->getMass();

		// Calculate the relative velocity
		vec3 relativeVelocity = col.objB->getVelocity() - col.objA->getVelocity();

		// Find out how much of the relative velocity goes along the collision vector
		float impactForce = glm::dot(relativeVelocity, col.collisionNormal);

		// Average elasticity of both objects
		float averageElasticity = (col.objA->getElasticity() + col.objB->getElasticity()) / 2;

		// Get the formula from our resources and calculate J
		float impulseMagnitude = (-(1 + averageElasticity) * impactForce) /  (inverseMassObjA + inverseMassObjB);

		// If both objects are spheres
		if (col.objA->getShapeType() == ShapeType::SPHERE && col.objB->getShapeType() == ShapeType::SPHERE)
		{
			Sphere * sphereA = (Sphere*)col.objA;
			Sphere * sphereB = (Sphere*)col.objB;
			float penetration = (sphereA->getRadius() + sphereB->getRadius()) - (glm::distance(col.objB->getPosition(), col.objA->getPosition()));

			if (col.objA->getIsStatic())
			{
				col.objB->setVelocity(col.objB->getVelocity() - (1 + col.objB->getElasticity()) * glm::dot(col.objB->getVelocity(), col.collisionNormal) * col.collisionNormal );
			}
			else
			{
				// Apply the J along the collision vector direction to object B
				col.objB->applyImpulse((col.collisionNormal * impulseMagnitude)* inverseMassObjB);
			}

			if (col.objB->getIsStatic())
			{

				col.objA->setVelocity(col.objA->getVelocity() - (1 + col.objA->getElasticity()) * glm::dot(col.objA->getVelocity(), col.collisionNormal) * col.collisionNormal);
			}
			else
			{
				// Apply the J against the collision vector direction to object A
				col.objA->applyImpulse((-col.collisionNormal * impulseMagnitude)* inverseMassObjA);
			}

			// Seperate the two objects, using whatever detail of seperation you want
			if(!col.objB->getIsStatic()) col.objB->setPosition(col.objB->getPosition() + (penetration / 2)* col.collisionNormal);
			if(!col.objA->getIsStatic()) col.objA->setPosition(col.objA->getPosition() - (penetration / 2)* col.collisionNormal);
		}
		else
		{
			// Cast to plane
			Plane* plane = (Plane*)col.objA;
			
			// Caclualte the velocity of the second object
			// Resulting velocity = velocity - (1 + elasticity)velocity.collision normal * collision normal
			vec3 velocity = col.objB->getVelocity() - (1 + col.objB->getElasticity())  * (glm::dot(col.objB->getVelocity(), plane->getDirection())) * plane->getDirection();

			// Set velocity
			col.objB->setVelocity(velocity);
		}
	}
	// Clears the vector as all collisions have been resolved
	m_collisions.clear();
}

