#include "PlayerControlledCarModel.h"
#include "CarModel.h"
#include "CubeModel.h"
#include "Renderer.h"
#include "SpotLight.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

PlayerControlledCarModel::PlayerControlledCarModel(
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
	vec3 lookAt = centerPosition + GetDirection();
	firstPersonCamera = new FirstPersonCamera(centerPosition);
	firstPersonCamera->SetLookAt(lookAt);
	World::GetInstance()->AddCamera(CameraType::CAMERA_FIRST, firstPersonCamera);
	thirdPersonCamera = new ThirdPersonCamera(centerPosition);
	thirdPersonCamera->SetLookAt(lookAt);
	World::GetInstance()->AddCamera(CameraType::CAMERA_THIRD, thirdPersonCamera);
}

PlayerControlledCarModel::~PlayerControlledCarModel() {
}

void PlayerControlledCarModel::Update(float dt) {
	CarModel::Update(dt);

	GLenum drawMode = GL_TRIANGLES;
	if (World::GetInstance()->GetCurrentCameraType() == CameraType::CAMERA_FIRST) {
		drawMode = GL_LINES;
	}

	for (auto it : cModels) {
		it->SetDrawMode(drawMode);
	}

	float horizontalAngle = mRotation.y;
	vec3 lookAt = GetLightDirection();

	firstPersonCamera->SetPosition(mCenterPosition);
	firstPersonCamera->SetLookAt(lookAt);
	thirdPersonCamera->SetPosition(mCenterPosition);
	thirdPersonCamera->SetHorizontalAngle(horizontalAngle);
	thirdPersonCamera->SetLookAt(lookAt);
}

