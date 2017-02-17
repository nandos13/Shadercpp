#include "GameObject.h"
#include "Application3D.h"
#include <limits>

glm::mat4 GameObject::getMatrixTransform()
{
	// Define radian
	float rad = 3.14f / 180.0f;
	glm::mat4 transform = glm::translate(m_translation)
		* glm::rotate(m_rotationEuler.z * rad, glm::vec3(0, 0, 1))
		* glm::rotate(m_rotationEuler.y * rad, glm::vec3(0, 1, 0))
		* glm::rotate(m_rotationEuler.x * rad, glm::vec3(1, 0, 0))
		* glm::scale(m_scale);

	return transform;
}

GameObject::GameObject()
{
	m_translation = glm::vec3(0);
	m_rotationEuler = glm::vec3(0);
	m_scale = glm::vec3(1);
}

GameObject::~GameObject()
{
}

void GameObject::Draw(glm::mat4 cameraMatrix, unsigned int programID)
{
	// Verify the model exists
	if (m_model != nullptr)
	{
		// Get Lighting information and pass into shader
		GetLights(programID);
		// Pass info in to draw model
		m_model->Draw(getMatrixTransform(), cameraMatrix, programID);
	}
}

void GameObject::GetLights(unsigned int programID)
{
	// Get the vector of scene lights
	std::vector<Light*> sceneLights = Application3D::GetLightSources();
	Light* closeLights[8];
	int size = sceneLights.size();
	if (size > 8)
	{
		// Loop through all lights and find the closest 8 light sources
		int i = sceneLights.size() - 1;
		while (i > 0)
		{
			// Sort the list by distance to the gameobject
			if (glm::distance(sceneLights[i]->GetPosition(), this->m_translation)
				< glm::distance(sceneLights[i - 1]->GetPosition(), this->m_translation))
			{
				// Swap the two elements
				std::iter_swap(sceneLights.begin()+i, sceneLights.begin()+i-1);
				// Move index up in case distance is larger than those already tested
				if (i != sceneLights.size() - 1)
					i++;
			}
			else
			{
				i--;
			}
		}
	}

	// Find how many objects need to be copied
	unsigned int quantity = (sceneLights.size() > 8) ? 8 : sceneLights.size();
	// Copy first 8 elements to the new array
	for (unsigned int j = 0; j < quantity; j++)
	{
		closeLights[j] = sceneLights[j];
	}

	// Pass in the amount of lights to be used
	unsigned int quantityUniform = glGetUniformLocation(programID, "lightQuantity");
	glUniform1i(quantityUniform, quantity);

	// Pass in the light positions & information (intensity, etc)
	glm::vec3 closeLightPositions[8];
	glm::vec2 closeLightInfo[8];
	for (int i = 0; i < quantity; i++)
	{
		if (closeLights[i] != nullptr)
		{
			closeLightPositions[i] = closeLights[i]->GetPosition();
			closeLightInfo[i] = glm::vec2(closeLights[i]->GetIntensity(), closeLights[i]->GetReach());
		}
	}
	// Positions
	unsigned int lightsUniform = glGetUniformLocation(programID, "lights");
	glUniform3fv(lightsUniform, 8, &closeLightPositions[0].x);
	// Other info
	unsigned int lightsInfoUniform = glGetUniformLocation(programID, "lightsInfo");
	glUniform2fv(lightsInfoUniform, 8, &closeLightInfo[0].x);
}

void GameObject::SetModel(Model * model)
{
	m_model = model;
}

Model * GameObject::GetModel()
{
	return m_model;
}

void GameObject::SetTranslation(glm::vec3 translation)
{
	m_translation = translation;
}

void GameObject::SetRotation(glm::vec3 euler)
{
	m_rotationEuler = euler;
}

void GameObject::SetScale(glm::vec3 localScale)
{
	m_scale = localScale;
}
