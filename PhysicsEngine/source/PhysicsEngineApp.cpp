#include "PhysicsEngineApp.h"
#include "Camera.h"
#include "Gizmos.h"
#include "Input.h"

#include "Physics/Scene.h"
#include "Physics/Object.h"
#include "Physics/Sphere.h"
#include "Physics/Plane.h"
#include "Physics/Spring.h"

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
	Gizmos::create(100000, 100000, 100000, 100000);

	// create simple camera transforms
	m_camera = new Camera();
	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	m_camera->SetPosition(glm::vec3(10, 10, 10));
	m_camera->Lookat(glm::vec3(0, 0, 0));
	
	// Create an scene
	m_scene = new Scene();

	// Make heavy object
	m_sphere = new Sphere(vec3(0.f,20.f,10.0f), 2.0f, 3.0f, vec4(0.2f, 0.1f, 0.7f, 0.9f), false);
	m_scene->addObject(m_sphere);
	//m_sphere->applyForce(vec3(1000.0f, 0.0f, 0.0f));

	// Make light object
	Sphere * sphere2 = new Sphere(vec3(20.0f, 20.0f, 10.0f),0.5f,1.0f, vec4(1.0f, 1.0f, 0.2f, 1.0f), false);
	m_scene->addObject(sphere2);


	Plane * plane = new Plane(0, vec3(0, 1, 0), vec4(0.2f, 1.0f, 0.2f, 0.7f));
	m_scene->addObject(plane);

	// Make static sphere
	Sphere * sphere3 = new Sphere(vec3(-3.0f, 10.f, 3.0f), 2.0f, 1.0f, vec4(1.0f, 1.0f, 0.2f, 1.0f), true);
	m_scene->addObject(sphere3);

	// Spring
	m_spring = new Spring(m_sphere, sphere2, 5.0f, 100.f, 1.f);
	m_scene->addSpring(m_spring);

	// Make Cloth
	MakeCloth(5, 5, vec3(0, 10, 0));

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
	//ImGui::Text("Velocity: %f", m_sphere->getVelocity().x);
	ImGui::Text("Assessment Mark: HD");
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
		Sphere * sphere3 = new Sphere(m_camera->GetPosition(), 1.f, 1.0f, vec4(0.4f, 0.5f, 0.1f, 0.8f), false);
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

void PhysicsEngineApp::MakeCloth(int rows, int columns, vec3 & origin,float radius, float springLength, float springDiagonal, float springCoefficient, float springDamping)
{
	vector<Object *> clothSpheres;
	vector<Spring *> clothSprings;
	Spring * clothSpring;
	Sphere * clothSphere;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			clothSphere = new Sphere(vec3(origin.x + (i), origin.y + (j), origin.z), radius, 0.1f, vec4(1.0f, 1.0f, 1.0f, 1.0f), isClothStaticSphere(i, rows, j, columns));
			clothSpheres.push_back(clothSphere);
		}
	}

	for (auto iter = clothSpheres.begin(); iter != clothSpheres.end(); iter++)
	{
		if (iter == clothSpheres.begin()) { int i = 0; }

	}

	int currentRow = 0;
	int currentColumn = 0;
	for (int i = 0; i < clothSpheres.size(); i++)
	{
		// If end of the row is reached
		if (currentColumn == columns)
		{
			currentColumn = 0;
			currentRow++;
		}

		// If we haven't reached the last in the row
		if (currentColumn < columns - 1)
		{
			// Next one in row
			m_spring = new Spring(clothSpheres[i], clothSpheres[i + 1], springLength, springCoefficient, springDamping);
			clothSprings.push_back(m_spring);
		}

		// Not the last row
		if (currentRow < rows - 1)
		{
			if (currentColumn > 0)
			{
				// Diagonal Left
				m_spring = new Spring(clothSpheres[i], clothSpheres[i + columns - 1], springDiagonal, springCoefficient, springDamping);
				clothSprings.push_back(m_spring);
			}

			if (currentColumn < columns - 1)
			{
				// Diagonal Right
				m_spring = new Spring(clothSpheres[i], clothSpheres[i + columns + 1], springDiagonal, springCoefficient, springDamping);
				clothSprings.push_back(m_spring);
			}

			// Up
			m_spring = new Spring(clothSpheres[i], clothSpheres[i + columns], springLength, springCoefficient, springDamping);
			clothSprings.push_back(m_spring);
		}

		currentColumn++;
	}
	for (auto sphere : clothSpheres)
	{
		m_scene->addObject(sphere);
	}

	for (auto springs : clothSprings)
	{
		m_scene->addSpring(springs);
	}
}

bool PhysicsEngineApp::isClothStaticSphere(int i, int rows, int j, int columns)
{
	return ((i == 0 || i == rows - 1) && j == columns -1) ? true : false;	
}
