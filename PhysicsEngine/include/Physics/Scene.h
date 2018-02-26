#pragma once
#include <vector>
#include <glm/glm.hpp>

using glm::vec3;
using std::vector;

namespace Physics {
	class Object;
	class Spring;

	// A struct to hold collisions that have been detected to be passed to the collision resolution 
	// function to be resolved. This holds pointers to the two objects that have collided and the collision normal.
	struct Collision 
	{
		Object * objA;
		Object * objB;
		vec3 collisionNormal;
	};

	/*
		The scene class handles the physics objects
	*/
	class Scene
	{
	public:
		// Constructor
		Scene();

		// Destructor
		~Scene();

		void update(float deltaTime);
		void draw();

		// Getter
		inline const vec3 & getGravity() const { return m_gravity; };
		inline const vec3 & getGlobalForce() const { return m_globalForce; }

		// Setter
		inline void setGravity(const vec3& gravity) { m_gravity = gravity; }
		inline void setGlobalForce(const vec3 & gForce) { m_globalForce = gForce; }

		// Add and remove object
		void addObject(Object * object);
		void removeObject(Object * object);

		// Add and remove spring
		void addSpring(Spring * spring);
		void removeSpring(Spring * spring);

		// Applies global force by applying the global force to all objects in the scene
		void applyGlobalForce();

	protected:
		vector<Object *> m_objects;
		vector<Collision> m_collisions;
		vector<Spring *> m_springs;

		vec3 m_gravity;
		vec3 m_globalForce;

		float m_fixedTimeStep;
		float m_accumulatedTime;
	private:
		void applyGravity();
		void checkCollision();
		void resolveCollision();
	};
}

