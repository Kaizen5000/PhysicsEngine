#include "Physics/Plane.h"
#include <Gizmos.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

using namespace Physics;
using glm::vec3;

Physics::Plane::Plane(float distance, vec3 direction, vec4 color) :  m_distance(distance), Object(Physics::ShapeType::PLANE, distance * direction, 0.f, color, true)
{
	// Ensures that the plane direction is normalised
	m_direction = glm::normalize(direction);
}

Plane::~Plane()
{
}

void Physics::Plane::draw()
{
	// Float for how far the plane stretches from the center
	float extents = 100.0f;

	// Creates a rotational matrix based on the plane normal and up vector
	glm::mat3 rot = glm::orientation(m_direction, vec3(0, 1, 0));

	// Calculates the vertices of the plane based on the extents and the rotation matrix
	vec3 tr = m_position + rot * vec3(extents, 0, extents);		// Top right
	vec3 br = m_position + rot * vec3(extents, 0, -extents);	// Back right
	vec3 bl = m_position + rot * vec3(-extents, 0, -extents);	// Back left
	vec3 tl = m_position + rot * vec3(-extents, 0, extents);	// Top left

	// Adds the triangles
	aie::Gizmos::addTri(tr, br, bl, m_color);
	aie::Gizmos::addTri(bl, tl, tr, m_color);
}
