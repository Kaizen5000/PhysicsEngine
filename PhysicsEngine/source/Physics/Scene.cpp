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
	m_accumulatedTime = 0.0f;
}


Scene::~Scene()
{
	// Delete all objects
	for (auto object : m_objects)
	{
		delete object;
	}
}

void Scene::update(float deltaTime)
{
	m_accumulatedTime += deltaTime;

	// Using fixed time step for update
	while (m_accumulatedTime >= m_fixedTimeStep)
	{
		applyGravity();
		for (auto object : m_objects)
		{
			object->update(m_fixedTimeStep);
		}
		for (auto spring : m_springs)
		{
			spring->update(m_fixedTimeStep);
		}
		m_accumulatedTime -= m_fixedTimeStep;
		checkCollision();
		resolveCollision();
	}

	
}

void Scene::draw()
{
	for (auto object : m_objects)
	{
		object->draw();
	}
	for (auto spring : m_springs)
	{
		spring->draw();
	}
}

void Scene::addObject(Object * object)
{
	m_objects.push_back(object);
}

void Scene::removeObject(Object * object)
{
	// Find object in vector
	auto iter = std::find(m_objects.begin(), m_objects.end(), object);
	// If found, delete
	if (iter != m_objects.end())
	{
		m_objects.erase(iter);
	}
}

void Physics::Scene::addSpring(Spring * spring)
{
	m_springs.push_back(spring);
}

void Physics::Scene::removeSpring(Spring * spring)
{
	// Find object in vector
	auto iter = std::find(m_springs.begin(), m_springs.end(), spring);
	// If found, delete
	if (iter != m_springs.end())
	{
		m_springs.erase(iter);
	}
}

void Scene::applyGlobalForce()
{
	for (auto object : m_objects)
	{
		object->applyForce(m_globalForce);
	}
}

void Scene::applyGravity()
{
	for (auto object : m_objects)
	{
		// Since gravity applies force based on mass
		object->applyForce(m_gravity* object->getMass());
	}
}

void Physics::Scene::checkCollision()
{
	// Find all collisions and place them in the collision vector
	for (auto object  = m_objects.begin(); object != m_objects.end(); object++)
	{
		// The second loop only checks after the first in the vector
		for (auto object2 = object + 1; object2 != m_objects.end(); object2++)
		{
			Collision tempCollision;
			if ((*object)->isColliding(*object2, tempCollision.collisionNormal))
			{
				if ((*object)->getShapeType() == SPHERE && (*object2)->getShapeType() == PLANE)
				{
					tempCollision.objA = *object2;
					tempCollision.objB = *object;
				}
				else
				{
					tempCollision.objA = *object;
					tempCollision.objB = *object2;
				}
				m_collisions.push_back(tempCollision);
			}
		}
	}
}

void Physics::Scene:: resolveCollision()
{

	for (auto col : m_collisions)
	{
		if (col.objA->getIsStatic() && col.objB->getIsStatic()) continue;
	
		// Inverse masses
		float inverseMassObjA = 1 / col.objA->getMass();
		float inverseMassObjB = 1 / col.objB->getMass();

		// Calculate the relative velocity
		vec3 relativeVelocity = col.objB->getVelocity() - col.objA->getVelocity();

		// Find out how much of the relative velocity goes along the collision vector
		float impactForce = glm::dot(relativeVelocity, col.collisionNormal);

		// Start putting together the impulse force
		// Elasticity ( this will be the object variable, might need to average it from both objects)
		float averageElasticity = (col.objA->getElasticity() + col.objB->getElasticity()) / 2;

		// Get the formula from our resources and calculate J
		float impulseMagnitude = (-(1 + averageElasticity) * impactForce) /  (inverseMassObjA + inverseMassObjB);

		if (col.objA->getShapeType() == SPHERE && col.objB->getShapeType() == SPHERE)
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
			Plane* plane = (Plane*)col.objA;
			// Resulting velocity = velocity - (1 + elasticity)velocity.collision normal * collision normal
			vec3 velocity = col.objB->getVelocity() - (1 + col.objB->getElasticity())  * (glm::dot(col.objB->getVelocity(), plane->getDirection())) * plane->getDirection();
			col.objB->setVelocity(velocity);
			
		}
	}
	m_collisions.clear();
}
