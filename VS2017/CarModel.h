#pragma once

#include "Model.h"
#include "CubeModel.h"
#include "CylinderModel.h"

class CarModel : public Model {
public:
	CarModel(
		glm::vec3 centerPosition = glm::vec3(0.0f, 0.25f, 0.0f), glm::vec3 centerShift = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 sizeScale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 shapeScale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f), float wheelAngle = 0.0f
	);
	virtual ~CarModel();

	virtual void Update(float dt);
	virtual void Draw();

	void SetIsMoving(bool isMoving);
	void Shift(float direction);
	void Turn(float direction);

	void SetVelocity(glm::vec3 velocity);

	void Reset();
	
	void UpdateWheels(float dt);
private:

	CubeModel *body;
	CubeModel *roof;
	CubeModel *bonnet;
	CubeModel *trunk;
	CylinderModel *wheels[4];
	CylinderModel *frontWheels[2];
	CylinderModel *backWheels[2];

	std::vector<Model*> cModels;

	glm::vec3 mVelocity;

	float mWheelAngle;
	bool mIsMoving;
};