#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "FreeCamera.h"
#include "tiny_obj_loader.h"
#include "Model.h"
#include "Light.h"

class GameObject;

struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
};

class Application3D : public aie::Application {
public:
	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	static std::vector<Light*> GetLightSources();

protected:

	void PassInLights();

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	FreeCamera* cam;
	unsigned int m_programID;
	unsigned int m_texture;

	unsigned int rows = 10;
	unsigned int cols = 10;

	static std::vector<Light*> m_lightSources;

	std::vector<GameObject*> m_sceneObjects;
};