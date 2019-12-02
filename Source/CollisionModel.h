#pragma once

#include "CubeModel.h"

class CollisionModel : public CubeModel {
public:
	CollisionModel(
		glm::vec3 centerPosition = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		float maxX = 0.5,
		float maxY = 0.5,
		float maxZ = 0.5
	);
	virtual ~CollisionModel();

	virtual void Update(float dt);
	virtual void Draw();
	virtual void GenerateModel();

	bool IsMovable() const { return mMovable; }
	glm::vec3 GetCollisionMax() const { return mMaxCollision; }

	void UpdateModel(
		float maxX,
		float maxY,
		float maxZ
	);
private:
	glm::vec3 mMaxCollision;

	float mMovable;
};