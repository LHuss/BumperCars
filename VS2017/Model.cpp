//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <math.h>

using namespace std;
using namespace glm;

Model::Model():
	mCenterPosition(0.0f, 0.0f, 0.0f), mCenterShift(1.0f, 1.0f, 1.0f), 
	mSizeScale(1.0f, 1.0f, 1.0f), mShapeScale(1.0f, 1.0f, 1.0f),
	mRotation(0.0f, 0.0f, 0.0f), mColor(0.5f, 0.5f, 0.5f),
	mDrawMode(GL_TRIANGLES), mRotationVelocity(0.0f, 0.0f, 0.0f),
	mTexture(TEXTURE_BRICK){
}

Model::~Model() {
}

void Model::Update(float dt) {
}

void Model::Draw() {
}

glm::mat4 Model::GetWorldMatrix() const {
	mat4 worldMatrix(1.0f);

	mat4 centerT = translate(mat4(1.0f), mCenterPosition);
	mat4 sizeS = scale(mat4(1.0f), mSizeScale);

	mat4 rot = mat4(1.0f)
		* rotate(mat4(1.0f), radians(mRotation.x), vec3(1.0f, 0.0f, 0.0f))
		* rotate(mat4(1.0f), radians(mRotation.y), vec3(0.0f, 1.0f, 0.0f))
		* rotate(mat4(1.0f), radians(mRotation.z), vec3(0.0f, 0.0f, 1.0f));

	mat4 shift = translate(mat4(1.0f), mCenterShift);

	mat4 shapeS = scale(mat4(1.0f), mShapeScale);

	worldMatrix = worldMatrix * centerT * sizeS * rot * shift * shapeS;

	return worldMatrix;
}

void Model::SetCenterPosition(glm::vec3 position) {
	mCenterPosition = position;
}

void Model::SetCenterShift(glm::vec3 shift) {
	mCenterShift = shift;
}

void Model::SetShapeScale(glm::vec3 shape) {
	mShapeScale = shape;
}

void Model::SetSizeScale(glm::vec3 size) {
	mSizeScale = size;
}

void Model::SetRotation(glm::vec3 rotation) {
	mRotation = vec3(fmod(rotation.x, 360), fmod(rotation.y, 360), fmod(rotation.z, 180));
}

void Model::SetColor(glm::vec3 color) {
	mColor = color;
}

void Model::SetColorFromRGB(int r, int g, int b) {
	mColor = ComputeColorFromRGB(r, g, b);
}

void Model::SetDrawMode(GLenum drawMode) {
	mDrawMode = drawMode;
}

void Model::SetTexture(TextureType texture) {
	mTexture = texture;
}

void Model::UpScale() {
	mSizeScale *= 1.05;
}

void Model::DownScale() {
	mSizeScale /= 1.05;
}

glm::vec3 Model::ComputeColorFromRGB(int r, int g, int b) {
	return glm::vec3((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
}
