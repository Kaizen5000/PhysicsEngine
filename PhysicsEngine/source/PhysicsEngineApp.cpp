#include "PhysicsEngineApp.h"
#include "Camera.h"

#include "Physics\Scene.h"
#include "Physics\Object.h"
#include "Physics\Sphere.h"
#include "Physics\Plane.h"

#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>


using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;
using namespace Physics;
PhysicsEngineApp::PhysicsEngineApp() {

}

PhysicsEngineApp::~PhysicsEngineApp() {

}

bool PhysicsEngineApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_camera = new Camera();
	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	m_camera->SetPosition(glm::vec3(10, 10, 10));
	m_camera->Lookat(glm::vec3(0, 0, 0));
	
	// Create an scene
	m_scene = new Scene();
	m_scene->setGlobalForce(vec3());
	//m_scene->setGravity(vec3());


	// Make heavy object
	m_sphere = new Sphere(vec3(0,20,0), 2.0f, 3.0f, vec4(0.2f, 0.1f, 0.7f, 0.9f), false);
	m_scene->addObject(m_sphere);
	//m_sphere->applyForce(vec3(1000.0f, 0.0f, 0.0f));


	

	// Make light object
	Sphere * sphere2 = new Sphere(vec3(20.0f, 20.0f, 0.0f),3.0f,1.0f, vec4(1.0f, 1.0f, 0.2f, 1.0f), false);
	m_scene->addObject(sphere2);


	Plane * plane = new Plane(0, vec3(0, 1, 0), vec4(0.2f, 1.0f, 0.2f, 0.7f));
	m_scene->addObject(plane);

	// Make static sphere
	Sphere * sphere3 = new Sphere(vec3(-3.0f, 10.f, 3.0f), 2.0f, 1.0f, vec4(1.0f, 1.0f, 0.2f, 1.0f), true);
	m_scene->addObject(sphere3);
	

	

	return true;
}

void PhysicsEngineApp::shutdown() {
	delete m_scene;
	delete m_camera;
	Gizmos::destroy();
}

void PhysicsEngineApp::update(float deltaTime) {

	m_camera->Update(deltaTime);
	ImGui::Begin("Debug");
	ImGui::Text("Velocity: %f", m_sphere->getVelocity().x);
	ImGui::End();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		Sphere * sphere3 = new Sphere(m_camera->GetPosition(), .5f, 1.0f, vec4(0.4f, 0.5f, 0.1f, 0.8f), false);
		m_scene->addObject(sphere3);
		sphere3->setVelocity(m_camera->getHeading() * 15.f);

	}
		
	m_scene->applyGlobalForce();
	m_scene->update(deltaTime);
}

void PhysicsEngineApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// Ask the scene to add all its Gizmos
	m_scene->draw();

	// update perspective based on screen size
	Gizmos::draw(m_camera->GetProjectionView());
}