#include "Physics/Plane.h"
#include <Gizmos.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

using namespace Physics;
using glm::vec3;

Physics::Plane::Plane(float distance, vec3 direction, vec4 color) : m_direction(direction), m_distance(distance), Object(Physics::ShapeType::PLANE, distance * direction, 0.f, color, true)
{
}

Plane::~Plane()
{
}

void Physics::Plane::draw()
{
	float extents = 100.0f;
	glm::mat3 rot = glm::orientation(m_direction, vec3(0, 1, 0));
	vec3 tr = m_position + rot * vec3(extents, 0, extents);
	vec3 br = m_position + rot * vec3(extents, 0, -extents);
	vec3 bl = m_position + rot * vec3(-extents, 0, -extents);
	vec3 tl = m_position + rot * vec3(-extents, 0, extents);
	aie::Gizmos::addTri(tr, br, bl, m_color);
	aie::Gizmos::addTri(bl, tl, tr, m_color);
}
