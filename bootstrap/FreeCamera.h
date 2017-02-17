#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>

class FreeCamera
{
private:
	glm::vec3 _position;
	float _fov;
	float _phi;
	float _theta;

	// Track mouse positions for rotation
	int lastMouseX;
	int lastMouseY;

public:
	FreeCamera();
	~FreeCamera();

	FreeCamera(glm::vec3 pos, float fov);
	void SetViewAngle(float phi, float theta);
	
	glm::mat4 GetProjectionMatrix(float w, float h);
	glm::mat4 GetViewMatrix();

	glm::vec3 GetPosition();

	void Update();
};

