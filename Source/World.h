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

#include <vector>
#include <list>

#include "CarModel.h"
#include "GridModel.h"
#include "Camera.h"
#include "Model.h"

struct LightSource {
	glm::vec3 position;
	glm::vec3 color;
};

class World
{
public:
	World();
	~World();

	static World* GetInstance();

	CubeModel* GetLight() const { return light; }

	void Update(float dt);
	void Draw();

	void InitializeModels();

	const Camera* GetCurrentCamera() const;

	static glm::mat4 GetInstancedViewProjectionMatrix();

private:
	static World* instance;

	CarModel* car;
	CubeModel* light;
	GridModel* grid;
	CubeModel* ground;
	std::list<CarModel*> projectileCars;
	std::vector<Model*> mobileModels;
	std::vector<Model*> staticModels;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
};