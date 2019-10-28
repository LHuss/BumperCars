//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "Camera.h"
#include <glm/gtx/transform.hpp>

#include "EventManager.h"

using namespace glm;

Camera::Camera() : mZoomFactor(1.0f)
{
}

Camera::~Camera()
{
}

mat4 Camera::GetViewProjectionMatrix() const
{
	// @TODO 1 - Calculate View Projection Matrix
	//           The projection matrix is hardcoded below
	//           The view matrix is set in the derived camera classes.

	mat4 viewProjection(1.0f);

	// Solution
#if 1
	viewProjection = GetProjectionMatrix() * GetViewMatrix();
#endif

	return viewProjection;
}

mat4 Camera::GetProjectionMatrix() const
{
	WindowResolution res = EventManager::GetWindowResolution();

	return perspective(radians(90.0f) * mZoomFactor, (float)res.width / (float)res.height, 0.1f, 100.0f);
}
