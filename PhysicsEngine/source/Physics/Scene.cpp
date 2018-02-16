#include "Physics\Scene.h"
#include "Physics\Object.h"
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
			if ((*object)->isColliding(*object2))
			{
				Collision tempCollision;
				tempCollision.objA = *object;
				tempCollision.objB = *object2;
				m_collisions.push_back(tempCollision);
			}
		}
	}
}

void Physics::Scene:: resolveCollision()
{
	/*
	for (auto col : m_collisions)
	{
		col.objA->applyForce(col.objB->getMass() * col.objB->getVelocity());
		col.objB->applyForce(col.objA->getMass() * col.objA->getVelocity());
	}
	*/
	for (auto col: m_collisions)
		{
			// Create a vector from A to B
		vec3 distance = col.objB->getPosition() - col.objA->getPosition();

			// Create a normalised collision vector
		vec3 nomalisedCollision = glm::normalize(distance);

			// Calculate the relative velocity
		vec3 relativeVelocity = col.objA->getVelocity() - col.objB->getVelocity();

			// Find out how much of the relative velocity goes along the collision vector
		float relativeAlongColisionVector = glm::dot(relativeVelocity, nomalisedCollision);

			// Start putting together the impulse force
			// Elasticity ( this will be the object variable, might need to average it from both objects)
		float averageElasticity = (col.objA->getElasticity() + col.objB->getElasticity()) / 2;
			// Get the formula from our resources and calculate J
		float impulseMagnitude = (-(1 + averageElasticity) * glm::dot(relativeVelocity, nomalisedCollision)) / (glm::dot(nomalisedCollision, nomalisedCollision * (1 / col.objA->getMass() + 1 / col.objB->getMass())));

			// Create a function for applyImpulse(vec3) in our object
			// This applies force without multiplying by deltatime

			// Apply the J along the collision vector direction to object B
		col.objB->applyImpulse(relativeVelocity* impulseMagnitude);
			// Apply the J against the collision vector direction to object A
		col.objA->applyImpulse(-relativeVelocity* impulseMagnitude);

			// Seperate the two objects, using whatever detail of seperation you want
		

		}
	m_collisions.clear();
}
