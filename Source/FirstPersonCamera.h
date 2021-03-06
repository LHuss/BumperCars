//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(glm::vec3 position);
	virtual ~FirstPersonCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

	void SetLookAt(glm::vec3 lookAt);
	void SetPosition(glm::vec3 position);
	void SetHorizontalAngle(float horizontalAngle);
	void SetVerticalAngle(float verticalAngle);

	glm::vec3 GetLookAt() const { return mLookAt; }

	float GetHorizontalAngle() const { return mHorizontalAngle; }
	float GetVerticalAngle() const { return mVerticalAngle; }

private:
	float mHorizontalAngle; // horizontal angle
	float mVerticalAngle;   // vertical angle

	float mSpeed;			// World units per second
	float mPanSpeed;
	float mTiltSpeed;
	float mZoomSpeed;
	float mAngularSpeed;    // Degrees per pixel

	bool mIsHoldingLeft;
	bool mIsHoldingRight;
	bool mIsHoldingMiddle;

	glm::vec3 mLookAt;
	glm::vec3 mFrozenLookAt;
	glm::vec3 mFrozenPosition;
};
