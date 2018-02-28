#pragma once
#include "Object.h"
#include <glm/glm.hpp>
using glm::vec3;
/*
	A plane class, which theoretically has infinite area
*/
namespace Physics
{
	class Plane : public Object
	{
	public:
		// Constructor requires a distance, direction, and colour
		Plane(float distance, vec3 direction, vec4 color);
		~Plane();
		
		// Draws the plane using gizmos; renders two triangles to show a complete rectangle
		void draw();
		
		// Getter
		inline const vec3 & getDirection() const { return m_direction; }
		inline const float getDistance() const { return m_distance; }

		// Setter
		inline void setDirection(const vec3 & direction) { m_direction = direction; }

	protected:
		// The direction that the plane is facing
		vec3 m_direction;
		
		// The distance that the plane is towards the direction
		float m_distance;
	};
}