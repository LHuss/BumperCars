//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

enum CameraType {
	CAMERA_FREE,
	CAMERA_FIRST,
	CAMERA_THIRD
};

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update(float dt) = 0;

	glm::vec3 GetPosition() const { return mPosition; }

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix() const;

protected:
	glm::vec3 mPosition;
	float mZoomFactor;

private:

};
