#pragma once
#include "Object.h"
namespace Physics
{
	class AABB : public Object
	{
	public:
		AABB(vec3 & position, vec3 & size, float mass, vec4 & color, bool isStatic);
		~AABB();
		void draw();

		// Getter
		inline const vec3 & getMinExtents() const { return m_minExtents; }
		inline const vec3 & getMaxExtents() const { return m_maxExtents; }
	protected:
		vec3 m_minExtents;
		vec3 m_maxExtents;
	};
}

