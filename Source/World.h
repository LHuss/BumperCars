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
#include "PlayerControlledCarModel.h"
#include "GridModel.h"
#include "Camera.h"
#include "Model.h"
#include "PointLight.h"
#include "SpotLight.h"

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

	std::vector<PointLight*> GetPointLights() const { return mPointLights; }
	std::vector<SpotLight*> GetSpotLights() const { return mSpotLights; }

	void Update(float dt);
	void Draw();

	void InitializeModels();

	const Camera* GetCurrentCamera() const;
	const CameraType GetCurrentCameraType() const;

	static glm::mat4 GetInstancedViewProjectionMatrix();

	void AddSpotLight(SpotLight* light);
	void RemoveSpotLight(SpotLight* light);
	void AddPointLight(PointLight* light);
	void RemovePointLight(PointLight* light);

	void AddCamera(CameraType type, Camera* camera);
	void ToggleCamera();

private:
	static World* instance;

	PlayerControlledCarModel* playerCar;
	GridModel* grid;
	CubeModel* ground;
	std::list<CarModel*> projectileCars;
	std::vector<Model*> mobileModels;
	std::vector<Model*> staticModels;
	std::vector<Camera*> mCameras;
	unsigned int mCurrentCamera;

	std::vector<PointLight*> mPointLights;
	std::vector<SpotLight*> mSpotLights;
};
