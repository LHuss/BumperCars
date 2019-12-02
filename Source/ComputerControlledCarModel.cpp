#include "ComputerControlledCarModel.h"
#include "CarModel.h"
#include "CubeModel.h"
#include "Renderer.h"
#include "SpotLight.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

ComputerControlledCarModel::ComputerControlledCarModel(
	glm::vec3 centerPosition, glm::vec3 centerShift,
	glm::vec3 sizeScale, glm::vec3 shapeScale,
	glm::vec3 rotation, glm::vec3 color,
	glm::vec3 velocity, float wheelAngle, float wheelSpin
) : CarModel(
	centerPosition, centerShift,
	sizeScale, shapeScale,
	rotation, color,
	velocity, wheelAngle, wheelSpin
) {
	mIsMoving = true;
	mCurrentStep = 0;
	mNumSteps = 10 + (rand() % 21);
}

ComputerControlledCarModel::~ComputerControlledCarModel() {
}

void ComputerControlledCarModel::Update(float dt) {
	CarModel::Update(dt);

	if (mCurrentStep == 0) {
		int direction = rand() % 2 == 0 ? 1 : -1;
		int amount = rand() % 180;
		Turn(direction * amount);
	}

	float carSpeed = 5.0f;
	float shiftAmount = carSpeed * dt;
	Shift(shiftAmount);

	mCurrentStep = (mCurrentStep + 1) % mNumSteps;
	if (mCurrentStep == 0) {
		mNumSteps = 10 + (rand() % 21);
	}
}

