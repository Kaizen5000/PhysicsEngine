#pragma once
#include "Object.h"
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
namespace Physics
{
	class Sphere : public Object
	{
	public:
		// Constructor
		Sphere(vec3 position, float radius, float mass, vec4 color, bool isStatic);

		// Destructor
		~Sphere();
		
		// Draws the sphere using gizmos
		void draw();

		// Getter
		inline float getRadius() const { return m_radius; };

		// Setter
		inline void setRadius(float radius) { m_radius = radius; }
	protected:
		// The radius of the sphere
		float m_radius;
	};
}

