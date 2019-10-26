//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "FirstPersonCamera.h"
#include "EventManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>



using namespace glm;

FirstPersonCamera::FirstPersonCamera(glm::vec3 position) : Camera(), mPosition(position), mLookAt(0.0f, 0.0f, -1.0f), mFrozenLookAt(mLookAt), mFrozenPosition(mPosition), mHorizontalAngle(90.0f), mVerticalAngle(0.0f), mSpeed(5.0f), mPanSpeed(2.0f), mTiltSpeed(3.0f), mZoomSpeed(0.05f), mAngularSpeed(2.5f), mIsHoldingLeft(false), mIsHoldingMiddle(false), mIsHoldingRight(false)
{
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Update(float dt)
{
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();


	// The Camera moves based on the User inputs
	// - You can access the mouse motion with EventManager::GetMouseMotionXY()
	// - For mapping A S D W, you can look in World.cpp for an example of accessing key states
	// - Don't forget to use dt to control the speed of the camera motion

	// Mouse motion to get the variation in angle
	double dx = EventManager::GetMouseMotionX();
	double dy = EventManager::GetMouseMotionY();
	
	mHorizontalAngle -= dx * mAngularSpeed * dt;
	mVerticalAngle -= dy * mAngularSpeed * dt;

	// Clamp vertical angle to [-85, 85] degrees
	mVerticalAngle = std::max(-85.0f, std::min(85.0f, mVerticalAngle));
	if (mHorizontalAngle > 360)
	{
		mHorizontalAngle -= 360;
	}
	else if (mHorizontalAngle < -360)
	{
		mHorizontalAngle += 360;
	}

	float theta = radians(mHorizontalAngle);
	float phi = radians(mVerticalAngle);

	mLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

	vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);

	GLFWwindow* window = EventManager::GetWindow();

	// A S D W for motion along the camera basis vectors
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mPosition += mLookAt * dt * mSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mPosition -= mLookAt * dt * mSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		mPosition += sideVector * dt * mSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		mPosition -= sideVector * dt * mSpeed;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (mIsHoldingLeft) {
			// mPosition -= mFrozenLookAt * (float)dy;
			mZoomFactor = max(0.5f, min(mZoomFactor + mZoomSpeed * (float)dy, 1.5f));
		}
		mIsHoldingLeft = true;
	}
	else {
		mIsHoldingLeft = false;
		mFrozenLookAt = mLookAt;
		mFrozenPosition = mPosition;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (mIsHoldingRight) {
			mPosition -= sideVector * (float)dx * mPanSpeed * dt;
		}
		mIsHoldingRight = true;
	}
	else {
		mIsHoldingRight = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		if (mIsHoldingMiddle) {
			mPosition += vec3(0.0f, dy, 0.0f) * mTiltSpeed * dt;
		}
		mIsHoldingMiddle = true;
	}
	else {
		mIsHoldingMiddle = false;
	}
}

glm::mat4 FirstPersonCamera::GetViewMatrix() const
{
	mat4 viewMatrix(1.0f);
	if (mIsHoldingLeft) {
		// viewMatrix = glm::lookAt(mPosition, mPosition + mFrozenLookAt, vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::lookAt(mFrozenPosition, mFrozenPosition + mFrozenLookAt, vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		viewMatrix = glm::lookAt(mPosition, mPosition + mLookAt, vec3(0.0f, 1.0f, 0.0f));
	}
	return viewMatrix;
}