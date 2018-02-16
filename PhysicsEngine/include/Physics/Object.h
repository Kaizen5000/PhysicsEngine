#pragma once
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
namespace Physics
{
	class Sphere;
	enum ShapeType {SPHERE, PLANE, AABB};
	class Object
	{
	protected:
		Object(ShapeType shape, vec3 pos, float mass, vec4 color, bool isStatic);
		

	public:

		void update(float deltaTime);
		virtual void draw() = 0;
		void applyForce(const vec3 & force);
		void applyImpulse(const vec3 & impulse);
		virtual ~Object() = 0;

		virtual bool isColliding(Object * other);


		// Getters
		inline const vec3 & getPosition() const { return m_position; }
		inline const vec3 & getVelocity() const { return m_velocity; }
		inline const vec3 & getAcceleration() const { return m_acceleration; }
		inline const float getMass() const { return m_mass; }
		inline const float getFriction() const { return m_friction; }
		inline const ShapeType getShapeType() const { return m_shape; }
		inline const float getElasticity() const { return m_elasticity; }

		// Setters
		inline void setPosition(const vec3 & pos) { m_position = pos; }
		inline void setVelocity(const vec3 & vel) { m_velocity = vel; }
		inline void setAcceleration(const vec3 & acc) { m_acceleration = acc; }
		inline void setMass(float mass) { m_mass = mass; }
		inline void setFriction(float friction) { m_friction = friction; }
		inline void setElasticity(float elasticity) { m_elasticity = elasticity;}

	protected:
		vec3 m_position;
		vec3 m_velocity;
		vec3 m_acceleration;
		ShapeType m_shape;
		float m_mass = 1.0f;
		float m_friction = 0.3f;
		float m_elasticity = 0.5;
		vec4 m_color;
		bool m_isStatic;

		bool isCollidingSphereSphere(Sphere * objA, Sphere * objB);
	};
}

