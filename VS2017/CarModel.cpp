#include "CarModel.h"
#include "CubeModel.h"
#include "Renderer.h"

using namespace glm;

CarModel::CarModel(
	vec3 centerPosition, vec3 centerShift,
	vec3 sizeScale, vec3 shapeScale,
	vec3 rotation, vec3 color,
	glm::vec3 velocity
) {
	mCenterPosition = centerPosition;
	mCenterShift = centerShift;
	mSizeScale = sizeScale;
	mShapeScale = shapeScale;
	mRotation = rotation;
	mColor = color;
	mVelocity = velocity;

	vec3 darkSlateGray = ComputeColorFromRGB(47, 79, 79);
	vec3 silver = ComputeColorFromRGB(192, 192, 192);
	vec3 gold = ComputeColorFromRGB(255, 215, 0);

	vec3 bodyShift = vec3(0.0f, 0.0f, 0.0f);
	vec3 bodyShape = vec3(2.0f, 1.0f, 1.0f) * shapeScale;
	body = new CubeModel(
		centerPosition, bodyShift,
		sizeScale, bodyShape,
		rotation, darkSlateGray
	);
	cModels.push_back(body);

	vec3 roofShift = vec3(0.0f, 1.0f, 0.0f);
	vec3 roofShape = vec3(1.0f, 0.5f, 0.5f) * shapeScale;
	roof = new CubeModel(
		centerPosition, roofShift,
		sizeScale, roofShape,
		rotation, silver
	);
	cModels.push_back(roof);

	vec3 bonnetShift = vec3(1.3625f, 0.0f, 0.0f);
	vec3 bonnetShape = vec3(0.75f, 0.6f, 1.0f) * shapeScale;
	bonnet = new CubeModel(
		centerPosition, bonnetShift,
		sizeScale, bonnetShape,
		rotation, silver
	);
	cModels.push_back(bonnet);

	vec3 trunkShift = vec3(-1.25f, 0.0f, 0.0f);
	vec3 trunkShape = vec3(0.5f, 0.8f, 1.0f) * shapeScale;
	trunk = new CubeModel(
		centerPosition, trunkShift,
		sizeScale, trunkShape,
		rotation, silver
	);
	cModels.push_back(trunk);


	float xPos[] = { -0.6f, 0.6f };
	float zPos[] = { -0.5f, 0.5f };
	int pos = 0;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			vec3 wheelShift = vec3(xPos[i], -0.25f, zPos[j]);
			vec3 wheelShape = vec3(0.5f, 0.5f, 0.5f) * shapeScale;
			wheels[pos] = new CubeModel(
				centerPosition, wheelShift,
				sizeScale, wheelShape,
				rotation, gold
			);
			cModels.push_back(wheels[pos]);
			pos++;
		}
	}
}


CarModel::~CarModel() {
	delete(body);
	body = NULL;
	delete(roof);
	roof = NULL;
	delete(bonnet);
	bonnet = NULL;
	delete(trunk);
	trunk = NULL;

	for (int i = 0; i < 4; i++) {
		delete(wheels[i]);
		wheels[i] = NULL;
	}

	cModels.clear();
}

void CarModel::Reset() {
	mCenterPosition = glm::vec3(0.0f, 0.25f, 0.0f);
	mCenterShift = glm::vec3(0.0f, 0.0f, 0.0f);
	mSizeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	mShapeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	mColor = glm::vec3(0.5f, 0.5f, 0.5f);
}

void CarModel::Update(float dt) {
	Model::Update(dt);

	mCenterPosition += mVelocity * dt;
	for(std::vector<Model*>::iterator it = cModels.begin(); it < cModels.end(); ++it)
	{
		(*it)->Update(dt);
		(*it)->SetCenterPosition(mCenterPosition);
		(*it)->SetRotation(mRotation);
		(*it)->SetDrawMode(mDrawMode);
		(*it)->SetSizeScale(mSizeScale);
		(*it)->SetDrawMode(mDrawMode);
	}
}

void CarModel::Draw() {
	for (std::vector<Model*>::iterator it = cModels.begin(); it < cModels.end(); ++it)
	{
		(*it)->Draw();
	}
}

void CarModel::SetVelocity(glm::vec3 velocity) {
	mVelocity = velocity;
}