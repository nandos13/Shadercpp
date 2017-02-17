#pragma once
#include "Model.h"
#include <gl_core_4_4.h>
#include <glm\ext.hpp>

class GameObject
{
private:
	Model* m_model;
	glm::vec3 m_translation;
	glm::vec3 m_rotationEuler;
	glm::vec3 m_scale;

	glm::mat4 getMatrixTransform();
public:
	GameObject();
	~GameObject();

	void Draw(glm::mat4 cameraMatrix, unsigned int programID);

	void GetLights(unsigned int programID);

	void SetModel(Model* model);
	Model* GetModel();

	void SetTranslation(glm::vec3 translation);
	void SetRotation(glm::vec3 euler);
	void SetScale(glm::vec3 localScale);
};

