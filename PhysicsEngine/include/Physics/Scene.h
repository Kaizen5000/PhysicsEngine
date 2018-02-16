#pragma once
#include <vector>
#include <glm/glm.hpp>
using glm::vec3;
using std::vector;

namespace Physics {
	class Object;
	struct Collision 
	{
		Object * objA;
		Object * objB;
	};
	class Scene
	{
	public:
		Scene();
		~Scene();

		void update(float deltaTime);
		void draw();
		inline const vec3 & getGravity() const { return m_gravity; };
		inline const vec3 & getGlobalForce() const { return m_globalForce; }

		inline void setGravity(const vec3& gravity) { m_gravity = gravity; }
		inline void setGlobalForce(const vec3 & gForce) { m_globalForce = gForce; }

		void addObject(Object * object);
		void removeObject(Object * object);
		void applyGlobalForce();
	protected:
		vector<Object *> m_objects;
		vec3 m_gravity;
		vec3 m_globalForce;

		vector<Collision> m_collisions;

		float m_fixedTimeStep;
		float m_accumulatedTime;
	private:
		void applyGravity();
		void checkCollision();
		void resolveCollision();
	};
}

