#pragma once

#include "Model.h"
#include "CubeModel.h"
#include "CylinderModel.h"
#include "SpotLight.h"
#include "CollisionModel.h"

class CarModel : public Model {
public:
	CarModel(
		glm::vec3 centerPosition = glm::vec3(0.0f, 0.25f, 0.0f), glm::vec3 centerShift = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 sizeScale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 shapeScale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f), float wheelAngle = 0.0f, float wheelSpin = 0.0f
	);
	virtual ~CarModel();

	virtual void Update(float dt);
	virtual void Draw();
	virtual void GenerateModel();

	void SetIsMoving(bool isMoving);
	void Shift(float direction);
	void Turn(float direction);

	void SetVelocity(glm::vec3 velocity);

	void Reset();
	void DisableLights();
	void EnableLights();
	void ToggleLights();
	void ShowCollisionBox();
	void HideCollisionBox();
	void ToggleCollisionBox();
	void EnableAnimation();
	void DisableAnimation();
	void ToggleAnimation();
	
	glm::vec3 GetDirection();
	glm::vec3 GetDirection(glm::vec3 rotation);
	glm::vec3 GetLightDirection();

	CollisionModel* GetCollisionBox() const { return collisionBox; }

	void InterpollateAnimationSpot(float dt);

protected:
	CubeModel *body;
	CubeModel *roof;
	CubeModel *bonnet;
	CubeModel *trunk;
	CylinderModel *wheels[4];
	CylinderModel *frontWheels[2];
	CylinderModel *backWheels[2];
	SpotLight* headLights[2];
	SpotLight* tailLights[2];
	SpotLight* lights[4];
	CollisionModel* collisionBox;

	std::vector<Model*> cModels;

	glm::vec3 mVelocity;

	float mWheelAngle;
	float mWheelSpin;
	float mMovementDirection;
	bool mIsMoving;

	bool mAnimationEnabled;
	glm::vec3 mAnimationSpotPoint;
	glm::vec3 mAnimationPoints[4];
	int mCurAnimationPoint;
};