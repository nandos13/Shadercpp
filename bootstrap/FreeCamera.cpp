#include "FreeCamera.h"
#include "Input.h"



FreeCamera::FreeCamera()
{
}


FreeCamera::~FreeCamera()
{
}

FreeCamera::FreeCamera(glm::vec3 pos, float fov)
{
	_position = pos;
	_fov = fov;
	_phi = 0;
	_theta = 0;
}

void FreeCamera::SetViewAngle(float phi, float theta)
{
	_phi = phi;
	_theta = theta;
}

glm::mat4 FreeCamera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, w/h, 0.1f, 1000.f);
}

glm::mat4 FreeCamera::GetViewMatrix()
{
	// Convert look angles to radians
	const float deg2rad = 3.14159f / 180;
	float thetaR = _theta * deg2rad;
	float phiR = _phi * deg2rad;
	glm::vec3 forward(cos(phiR)*cos(thetaR), sin(phiR), cos(phiR)*sin(thetaR));
	return glm::lookAt(_position, _position + forward, glm::vec3(0,1,0));
}

glm::vec3 FreeCamera::GetPosition()
{
	return _position;
}

void FreeCamera::Update()
{
	aie::Input* input = aie::Input::getInstance();
	const float deg2rad = 3.14159f / 180;
	float thetaR = _theta * deg2rad;
	float phiR = _phi * deg2rad;

	/* POSITION & ROTATION */

	// calculate the forwards and right axes and up axis for the camera
	glm::vec3 forward(cos(phiR)*cos(thetaR), sin(phiR), cos(phiR)*sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	

	/* ROTATION */

	float mx = input->getMouseX();
	float my = input->getMouseY();

	// Check right mouse is down
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		// position
		float dTime = 0.1f;
		float speed = 1;
		if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
			speed = 2.5f;

		// use WASD, ZX keys to move camera around
		if (input->isKeyDown(aie::INPUT_KEY_X))
			_position += up * dTime * speed;
		if (input->isKeyDown(aie::INPUT_KEY_Z))
			_position += -up * dTime * speed;
		if (input->isKeyDown(aie::INPUT_KEY_A))
			_position += -right * dTime * speed;
		if (input->isKeyDown(aie::INPUT_KEY_D))
			_position += right * dTime * speed;
		if (input->isKeyDown(aie::INPUT_KEY_W))
			_position += forward * dTime * speed;
		if (input->isKeyDown(aie::INPUT_KEY_S))
			_position += -forward * dTime * speed;

		// rotation
		_theta += 0.2f * (mx - lastMouseX);
		_phi += 0.2f * (my - lastMouseY);

		// clamp values
		if (_phi > 85) _phi = 85;
		if (_phi < -85) _phi = -85;
	}

	lastMouseX = mx;
	lastMouseY = my;
}
