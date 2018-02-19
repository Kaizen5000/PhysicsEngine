#pragma once
#include "Object.h"
#include <glm/glm.hpp>
using glm::vec3;
namespace Physics
{
	class Plane : public Object
	{
	public:
		Plane(float distance, vec3 direction, vec4 color);
		~Plane();
		void draw();
		inline const vec3 & getDirection() const { return m_direction; }
		inline const float getDistance() const { return m_distance; }

		inline void setDirection(const vec3 & direction) { m_direction = direction; }
	protected:
		vec3 m_direction;
		float m_distance;
	};
}

