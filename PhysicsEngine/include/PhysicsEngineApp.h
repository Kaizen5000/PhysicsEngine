#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class Camera;
namespace Physics {
	class Scene;
	class Sphere;
	class Spring;
}


class PhysicsEngineApp : public aie::Application {
public:

	PhysicsEngineApp();
	virtual ~PhysicsEngineApp();
	virtual bool startup();
	virtual void shutdown();
	virtual void update(float deltaTime);
	virtual void draw();
protected:	
	Camera *m_camera = nullptr;

	// Function that creates cloth based on input parameters, spring variables have default values
	void MakeCloth(int rows, int columns, glm::vec3 & origin,float radius = 0.1f, float springLength = 1.f, float springDiagonal = 1.4f, float springCoefficient = 10.f, float springDamping = 0.2f);

	Physics::Scene * m_scene = nullptr;
	Physics::Sphere * m_sphere;
	Physics::Spring * m_spring;
};