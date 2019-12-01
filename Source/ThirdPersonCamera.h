#pragma once

#include "Camera.h"

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(glm::vec3 position);
	virtual ~ThirdPersonCamera();

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
	float radius = 5.0f;
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
