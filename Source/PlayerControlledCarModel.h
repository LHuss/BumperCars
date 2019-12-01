#pragma once

#include "CarModel.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"

class PlayerControlledCarModel : public CarModel {
public:
	PlayerControlledCarModel(
		glm::vec3 centerPosition = glm::vec3(0.0f, 0.25f, 0.0f), glm::vec3 centerShift = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 sizeScale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 shapeScale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f), float wheelAngle = 0.0f, float wheelSpin = 0.0f
	);
	virtual ~PlayerControlledCarModel();

	virtual void Update(float dt);
private:
	FirstPersonCamera* firstPersonCamera;
	ThirdPersonCamera* thirdPersonCamera;
};