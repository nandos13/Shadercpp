#include "Light.h"



Light::Light()
{
	m_position = glm::vec3(0);
	m_distance = 10;
	m_intensity = 0.8f;
}


Light::~Light()
{
}

void Light::SetPosition(glm::vec3 pos)
{
	m_position = pos;
}

void Light::SetIntensity(float intensity)
{
	m_intensity = intensity;
	if (m_intensity > 1)
		m_intensity = 1;
	if (m_intensity < 0)
		m_intensity = 0;
}

void Light::SetReach(float reach)
{
	m_distance = reach;
}

glm::vec3 Light::GetPosition()
{
	return m_position;
}

float Light::GetIntensity()
{
	return m_intensity;
}

float Light::GetReach()
{
	return m_distance;
}
