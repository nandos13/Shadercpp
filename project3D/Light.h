#pragma once
#include <glm\ext.hpp>

class Light
{
protected:
	glm::vec3 m_position;
	float m_distance;
	float m_intensity;
public:
	Light();
	~Light();

	void SetPosition(glm::vec3 pos);
	void SetIntensity(float intensity);
	void SetReach(float reach);

	glm::vec3 GetPosition();
	float GetIntensity();
	float GetReach();
};

