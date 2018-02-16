#include "..\..\include\Physics\Sphere.h"
#include <Gizmos.h>
using namespace Physics;

Physics::Sphere::Sphere(vec3 position, float radius, float mass, vec4 color, bool isStatic) : 
	m_radius(radius), Object(Physics::ShapeType::SPHERE, position, mass, color, isStatic)
{
}

Sphere::~Sphere()
{
}

void Physics::Sphere::draw()
{
	aie::Gizmos::addSphere(m_position, m_radius, 12, 12, m_color);
}
