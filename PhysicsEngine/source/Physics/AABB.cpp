#include "Physics/AABB.h"
#include <Gizmos.h>
using namespace Physics;
using glm::vec3;
Physics::AABB::AABB(vec3  position, vec3 size, float mass, vec4 color, bool isStatic): m_extents(size), Object(ShapeType::AABB,position,mass, color, isStatic)
{
}

AABB::~AABB()
{
}

void Physics::AABB::draw()
{
	// Create a filled AABB
	aie::Gizmos::addAABBFilled(m_position, m_extents, m_color);
}

vec3 Physics::AABB::getMin()
{
	return m_position - m_extents;
}

vec3 Physics::AABB::getMax()
{
	return m_position + m_extents;
}
