#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include "Shader.h"
#include <Texture.h>
#include "GameObject.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

std::vector<Light*> Application3D::m_lightSources;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);
	cam = new FreeCamera(vec3(-10, 10, 0), 90);
	cam->SetViewAngle(-30, 0);

	// Set random seed
	srand(time(NULL));

	// Spawn random lights
	//int numberOfLights = 8;
	//for (int i = 0; i < numberOfLights; i++)
	//{
	//	Light* light = new Light();
	//	light->SetPosition(glm::vec3(rand() % 160 + -80, rand() % 40 + -20, rand() % 160 + -80));
	//	light->SetIntensity( (float)(rand() % 96 + 10) / 100 );
	//	light->SetReach(rand() % 10 + 30);
	//	m_lightSources.push_back(light);
	//}
	Light* light = new Light();
	light->SetPosition(glm::vec3(20, 2, 0));
	light->SetIntensity(0.5f);
	light->SetReach(30);
	m_lightSources.push_back(light);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	//char* vs = "data/shaders/BasicVertexShader.txt";
	//char* fs = "data/shaders/DiffuseLightTestFragment.txt";
	char* vs = "data/shaders/vtest.txt";
	char* fs = "data/shaders/fnew.txt";
	m_programID = Shader::CompileShaders(vs, fs);

	Model* testModel = new Model();
	testModel->Load("data/models/handgun_obj.obj");
	float scale = 1;
	testModel->CreateBuffers(scale);

	char* textureLocation = "data/textures/handgun/handgun_C2.jpg";
	testModel->LoadNewTexture(textureLocation);

	GameObject* go = new GameObject();
	go->SetModel(testModel);
	m_sceneObjects.push_back(go);
	go->SetTranslation(vec3(0, 1, 0));
	go->SetRotation(vec3(0, 90, 0));

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// update cam
	cam->Update();

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
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = cam->GetProjectionMatrix((float)getWindowWidth(), (float)getWindowHeight());
	m_viewMatrix = cam->GetViewMatrix();

	glUseProgram(m_programID);

	// Apply ambient light
	float ambientLight = 0.12f;
	unsigned int ambientUniform = glGetUniformLocation(m_programID, "ambientAmount");
	glUniform1f(ambientUniform, ambientLight);

	// Pass in camera position
	unsigned int cameraPosUniform = glGetUniformLocation(m_programID, "cameraPos");
	glUniform4f(cameraPosUniform, cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z, 0);


	// Draw light sources
	for (int i = 0; i < (int)m_lightSources.size(); i++)
	{
		Gizmos::addSphere(m_lightSources[i]->GetPosition(), 0.3f, 16, 16, vec4(1, 1, 0, 1));
	}

	// Draw all scene objects
	for (int i = 0; i < (int)m_sceneObjects.size(); i++)
	{
		if (m_sceneObjects[i] != nullptr)
		{
			m_sceneObjects[i]->Draw(m_projectionMatrix * m_viewMatrix, m_programID);
		}
	}

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

std::vector<Light*> Application3D::GetLightSources()
{
	return m_lightSources;
}

void Application3D::PassInLights()
{
	// Pass in the number of lights to be used
	int size = m_lightSources.size();
	if (size > 8)
	{
		// Loop through all lights and find the closest 8 light sources
		
	}
	unsigned int quantityUniform = glGetUniformLocation(m_programID, "lightQuantity");
	glUniform1i(quantityUniform, m_lightSources.size());
}
