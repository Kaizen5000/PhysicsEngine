#include "Physics/AABB.h"
using namespace Physics;
#include <Gizmos.h>
Physics::AABB::AABB(vec3 & position, vec3 & size, float mass, vec4 & color, bool isStatic): m_minExtents(position), Object(ShapeType::AABB,position,mass, color, isStatic)
{
	m_maxExtents = position + size;
}

AABB::~AABB()
{
}

void Physics::AABB::draw()
{
	vec3 temp = m_maxExtents - m_minExtents;
	aie::Gizmos::addAABBFilled(vec3(temp.x / 2, temp.y / 2, temp.z / 2), vec3(m_minExtents.x / 2, m_minExtents.y / 2, m_minExtents.z / 2), m_color);

}
