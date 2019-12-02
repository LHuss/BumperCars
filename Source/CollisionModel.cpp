#pragma once

#include "CollisionModel.h"
#include "CubeModel.h"

using namespace glm;

CollisionModel::CollisionModel(
	glm::vec3 centerPosition,
	glm::vec3 rotation,
	float maxX,
	float maxY,
	float maxZ
) {
	CubeModel();

	mHidden = true;
	SetDrawMode(GL_LINES);
	SetTexture(TextureType::TEXTURE_NULL);

	UpdateModel(maxX, maxY, maxZ);

	GenerateModel();
}

CollisionModel::~CollisionModel() {
}

void CollisionModel::Update(float dt) {
	CubeModel::Update(dt);
}

void CollisionModel::Draw() {
	if (!mHidden) {
		CubeModel::Draw();
	}
}

void CollisionModel::GenerateModel() {
	CubeModel::GenerateModel();
}

void CollisionModel::UpdateModel(
	float maxX,
	float maxY,
	float maxZ
) {
	vec3 maxes = vec3(max(mMax.x, abs(mMin.x)), max(mMax.y, abs(mMin.y)), max(mMax.z, abs(mMin.z)));
	mShapeScale = vec3(maxX / maxes.x, maxY / maxes.y, maxZ / maxes.z);
	mMaxCollision = vec3(maxX, maxY, maxZ);
}
