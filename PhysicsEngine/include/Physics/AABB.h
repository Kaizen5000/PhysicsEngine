#pragma once
#include "Object.h"
namespace Physics
{
	class AABB : public Object
	{
	public:
		AABB(vec3 position, vec3 halfExtent, float mass, vec4 color, bool isStatic);
		~AABB();
		void draw();

		// Getter
		inline const vec3 & getExtents() const { return m_extents; }
		vec3 getMin();
		vec3 getMax();
	protected:
		vec3 m_extents;
	};
}

