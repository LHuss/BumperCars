#include "CarModel.h"
#include "CubeModel.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

CarModel::CarModel(
	vec3 centerPosition, vec3 centerShift,
	vec3 sizeScale, vec3 shapeScale,
	vec3 rotation, vec3 color,
	glm::vec3 velocity, float wheelAngle,
	float wheelSpin
) {
	mCenterPosition = centerPosition;
	mCenterShift = centerShift;
	mSizeScale = sizeScale;
	mShapeScale = shapeScale;
	mRotation = rotation;
	mColor = color;
	mVelocity = velocity;
	mWheelAngle = wheelAngle;
	mWheelSpin = wheelSpin;
	mMovementDirection = 0.0f;
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

	for (auto it : wheels) {
		delete(it);
		it = NULL;
	}

	cModels.clear();
}

void CarModel::GenerateModel() {
	vec3 darkSlateGray = ComputeColorFromRGB(47, 79, 79);
	vec3 silver = ComputeColorFromRGB(192, 192, 192);
	vec3 gold = ComputeColorFromRGB(255, 215, 0);

	vec3 bodyShift = vec3(0.0f, 0.0f, 0.0f);
	vec3 bodyShape = vec3(2.0f, 1.0f, 1.0f) * mShapeScale;
	body = new CubeModel(
		mCenterPosition, bodyShift,
		mSizeScale, bodyShape,
		mRotation, darkSlateGray
	);
	body->SetTexture(TextureType::TEXTURE_STEEL);
	body->GenerateModel();
	cModels.push_back(body);

	vec3 roofShift = vec3(0.0f, 1.0f, 0.0f);
	vec3 roofShape = vec3(1.0f, 0.5f, 0.5f) * mShapeScale;
	roof = new CubeModel(
		mCenterPosition, roofShift,
		mSizeScale, roofShape,
		mRotation, silver
	);
	roof->SetTexture(TextureType::TEXTURE_STEEL);
	roof->GenerateModel();
	cModels.push_back(roof);

	vec3 bonnetShift = vec3(1.3625f, 0.0f, 0.0f);
	vec3 bonnetShape = vec3(0.75f, 0.6f, 1.0f) * mShapeScale;
	bonnet = new CubeModel(
		mCenterPosition, bonnetShift,
		mSizeScale, bonnetShape,
		mRotation, silver
	);
	bonnet->SetTexture(TextureType::TEXTURE_STEEL);
	bonnet->GenerateModel();
	cModels.push_back(bonnet);

	vec3 trunkShift = vec3(-1.25f, 0.0f, 0.0f);
	vec3 trunkShape = vec3(0.5f, 0.8f, 1.0f) * mShapeScale;
	trunk = new CubeModel(
		mCenterPosition, trunkShift,
		mSizeScale, trunkShape,
		mRotation, silver
	);
	trunk->SetTexture(TextureType::TEXTURE_WOOD);
	trunk->GenerateModel();
	cModels.push_back(trunk);


	float xPos[] = { 0.6f, -0.6f };
	float zPos[] = { -0.5f, 0.5f };
	int pos = 0;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			vec3 wheelShift = vec3(xPos[i], -0.25f, zPos[j]);
			vec3 wheelShape = vec3(0.5f, 0.5f, 0.25f) * mShapeScale;
			wheels[pos] = new CylinderModel(
				mCenterPosition, wheelShift,
				mSizeScale, wheelShape,
				mRotation, gold
			);
			wheels[pos]->SetTexture(TextureType::TEXTURE_TIRE);
			wheels[pos]->GenerateModel();

			cModels.push_back(wheels[pos]);

			if (i == 0) {
				frontWheels[j] = wheels[pos];
			}
			else {
				backWheels[j] = wheels[pos];
			}

			pos++;
		}
	}
}

void CarModel::Reset() {
	mCenterPosition = glm::vec3(0.0f, 0.25f, 0.0f);
	mCenterShift = glm::vec3(0.0f, 0.0f, 0.0f);
	mSizeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	mShapeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	mRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	mColor = glm::vec3(0.5f, 0.5f, 0.5f);
	mWheelAngle = 0.0f;
}

void CarModel::Update(float dt) {
	Model::Update(dt);

	if (mIsMoving) {
		float carTurnSpeed = 50.0f;
		float carTurnAmount = carTurnSpeed * dt;
		float direction = mWheelAngle == 0 ? 0 : mWheelAngle / abs(mWheelAngle);

		float maxCarCanTurn = min(abs(mWheelAngle), carTurnAmount) * direction * mMovementDirection;
		vec3 maxRot = vec3(0.0f, 1.0f, 0.0f) * maxCarCanTurn;

		float newWheelAngle = mWheelAngle - (maxCarCanTurn * mMovementDirection);
		mWheelAngle = newWheelAngle;

		vec3 newCarRot = mRotation + maxRot;
		mRotation = newCarRot;


		float wheelSpinSpeed = 180.0f;
		float wheelSpinAmount = wheelSpinSpeed * dt * mMovementDirection;
		mWheelSpin = fmod(mWheelSpin + wheelSpinAmount, 360.0f);
	}

	vec3 wheelSpin = vec3(0.0f, 0.0f, -1.0f) * mWheelSpin;
	vec3 wheelTurn = vec3(0.0f, 1.0f, 0.0f) * mWheelAngle;
	vec3 frontWheelRotation = wheelTurn + wheelSpin;

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

	/*for (auto it : wheels) {
		it->SetPointRotation(wheelSpin);
	}*/

	for (auto it : frontWheels) {
		it->SetPointRotation(wheelTurn);
		//it->SetPointRotation(frontWheelRotation);
	}
}

void CarModel::Draw() {
	if (mHidden) {
		return;
	}

	for (std::vector<Model*>::iterator it = cModels.begin(); it < cModels.end(); ++it)
	{
		(*it)->Draw();
	}
}

void CarModel::Shift(float direction) {
	mMovementDirection = direction > 0.0f ? 1.0f : direction < 0.0f ? -1.0f : 0.0f;

	vec3 distance = vec3(1.0f, 0.0f, 0.0f) * direction;
	vec4 distanceShift = vec4(distance.x, distance.y, distance.z, 1)
		* rotate(mat4(1.0f), radians(mRotation.x), vec3(1.0f, 0.0f, 0.0f))
		* rotate(mat4(1.0f), radians(mRotation.y), vec3(0.0f, 1.0f, 0.0f))
		* rotate(mat4(1.0f), radians(mRotation.z), vec3(0.0f, 0.0f, 1.0f));
	mCenterPosition += vec3(distanceShift.x, distanceShift.y, -distanceShift.z);
}

void CarModel::Turn(float direction) {
	mWheelAngle = min(max((mWheelAngle + direction), -60.0f), 60.0f);
}

void CarModel::SetIsMoving(bool isMoving) {
	mIsMoving = isMoving;
}

void CarModel::SetVelocity(vec3 velocity) {
	mVelocity = velocity;
}