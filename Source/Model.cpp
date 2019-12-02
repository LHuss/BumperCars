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

Model::Model() :
	mCenterPosition(0.0f, 0.0f, 0.0f), iCenterPosition(mCenterPosition),
	mCenterShift(1.0f, 1.0f, 1.0f), iCenterShift(mCenterShift),
	mSizeScale(1.0f, 1.0f, 1.0f), iSizeScale(mSizeScale),
	mShapeScale(1.0f, 1.0f, 1.0f), iShapeScale(mShapeScale),
	mRotation(0.0f, 0.0f, 0.0f), iRotation(mRotation),
	mPointRotation(0.0f, 0.0f, 0.0f), iPointRotation(mPointRotation),
	mColor(0.5f, 0.5f, 0.5f), iColor(mColor),
	mDrawMode(GL_TRIANGLES), mRotationVelocity(0.0f, 0.0f, 0.0f),
	mTexture(TextureType::TEXTURE_NULL), mSpecificShader(ShaderType::SHADER_NULL),
	mHidden(false) {
}

Model::~Model() {
}

void Model::Update(float dt) {
}

void Model::Draw() {
}

void Model::GenerateModel() {
}

glm::mat4 Model::GetWorldMatrix() const {
	mat4 worldMatrix(1.0f);

	mat4 centerT = translate(mat4(1.0f), mCenterPosition);

	mat4 sizeS = scale(mat4(1.0f), mSizeScale);

	mat4 rot = ComputeRotationMatrix(mRotation);

	mat4 shift = translate(mat4(1.0f), mCenterShift);

	mat4 shapeS = scale(mat4(1.0f), mShapeScale);

	mat4 pointRot = ComputeRotationMatrix(mPointRotation);

	worldMatrix = worldMatrix * centerT * sizeS * rot * shift * shapeS * pointRot;

	return worldMatrix;
}

mat4 Model::ComputeRotationMatrix(vec3 rotation) {
	return mat4(1.0f)
		* rotate(mat4(1.0f), radians(rotation.x), vec3(1.0f, 0.0f, 0.0f))
		* rotate(mat4(1.0f), radians(rotation.y), vec3(0.0f, 1.0f, 0.0f))
		* rotate(mat4(1.0f), radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
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
	mRotation = BindRotation(rotation);
}

void Model::SetPointRotation(glm::vec3 rotation) {
	mPointRotation = BindRotation(rotation);
}

vec3 Model::BindRotation(glm::vec3 rotation) {
	float x =  fmodf(rotation.x, 360);
	if (x < 0) {
		x += 360;
	}
	float y = fmodf(rotation.y, 360);
	if (y < 0) {
		y += 360;
	}
	float z = fmodf(rotation.z, 360);
	if (z < 0) {
		z += 360;
	}
	return vec3(x, y, z);
}

void Model::SetColor(glm::vec3 color) {
	mColor = color;
}

void Model::SetColorFromRGB(int r, int g, int b) {
	mColor = ComputeColorFromRGB(r, g, b);
}

void Model::SetColorFromVec3(vec3 rgb) {
	mColor = ComputeColorFromRGB(rgb.x, rgb.y, rgb.z);
}

void Model::SetDrawMode(GLenum drawMode) {
	mDrawMode = drawMode;
}

void Model::SetTexture(TextureType texture) {
	mTexture = texture;
}

void Model::SetSpecificShader(ShaderType shader) {
	mSpecificShader = shader;
}

void Model::UpScale() {
	mSizeScale *= 1.05;
}

void Model::DownScale() {
	mSizeScale /= 1.05;
}

void Model::Show() {
	mHidden = false;
}

void Model::Hide() {
	mHidden = true;
}

void Model::Toggle() {
	mHidden = !mHidden;
}

glm::vec3 Model::ComputeColorFromRGB(int r, int g, int b) {
	return glm::vec3((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
}
