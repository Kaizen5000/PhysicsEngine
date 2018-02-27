#pragma once
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;


namespace Physics
{
	// Forward declaration
	class Sphere;
	class Plane;
	class AABB;

	// ShapeType enum to identify the shape of the object
	enum class ShapeType {SPHERE, PLANE, AABB};

	/*
	The object class is a base class for objects in the scene such as spheres and planes.
	This class is pure virtual as it is not intended to instantiated on its own
	*/
	class Object
	{
	protected:
		// Protected constructor
		Object(ShapeType shape, vec3 pos, float mass, vec4 color, bool isStatic);
	public:
		// Update takes deltaTime as an parameter and is responsible for updating the position of the object based on acceleration and velocity
		void update(float deltaTime);
		
		// This function is used to apply a force to the object, increasing the acceleration relative to the mass
		void applyForce(const vec3 & force);

		// Used to add immediate force without taking into account mass and deltaTime
		void applyImpulse(const vec3 & impulse);

		// Pure virtual draw function as different objects will draw differently
		virtual void draw() = 0;
		
		// Virtual destructor as this is a base class
		virtual ~Object();

		// This function returns a boolean if this object is colliding with the object passed through as an object pointer
		// A reference to a collision normal variable is passed through to be edited when the collision detection is calculated
		virtual bool isColliding(Object * other, vec3 & collisionNormal);


		// Getters
		inline const vec3 & getPosition() const { return m_position; }
		inline const vec3 & getVelocity() const { return m_velocity; }
		inline const vec3 & getAcceleration() const { return m_acceleration; }
		inline const float getMass() const { return m_mass; }
		inline const float getFriction() const { return m_friction; }
		inline const ShapeType getShapeType() const { return m_shape; }
		inline const float getElasticity() const { return m_elasticity; }
		inline const bool getIsStatic() const { return m_isStatic; }

		// Setters
		inline void setPosition(const vec3 & pos) { m_position = pos; }
		inline void setVelocity(const vec3 & vel) { m_velocity = vel; }
		inline void setAcceleration(const vec3 & acc) { m_acceleration = acc; }
		inline void setMass(float mass) { m_mass = mass; }
		inline void setFriction(float friction) { m_friction = friction; }
		inline void setElasticity(float elasticity) { m_elasticity = elasticity;}

	protected:
		vec3 m_position;			// The position of the object
		vec3 m_velocity;			// The current velocity of the object
		vec3 m_acceleration;		// The acceleration of the object
		ShapeType m_shape;			// The shape type of the object
		float m_mass = 1.0f;		// The mass of the object
		float m_friction = 0.3f;	// A value between 0 and 1 to determine how much velocity is lost each tick
		float m_elasticity = 1.f;	// Determines how much of the collision velocity is retained 
		vec4 m_color;				// The RBG colour of the object
		bool m_isStatic;			// Bool to determine if the object is static

		// These functions check whether the respective objects are colliding
		// They are called from the isColliding method after both objects are identified
		// The collision normal reference is further passed into these functions because the collision will be calculate and the collision normal can be assigned
		bool isCollidingSphereSphere(Sphere * objA, Sphere * objB, vec3 &collisionNormal);
		bool isCollidingPlaneSphere(Plane * objA, Sphere * objB, vec3 &collisionNormal);

		// TODO: Implement these collision function
		bool isCollidingPlaneAABB(Plane * objA, AABB * objB, vec3 &collisionNormal);
		bool isCollidingAABBAABB(AABB * objA, AABB * objB, vec3 &collisionNormal);
		bool isCollidingSphereAABB(Sphere * objA, AABB * objB, vec3 &collisionNormal);
	};
}

