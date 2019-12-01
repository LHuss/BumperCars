//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"

#include "FirstPersonCamera.h"
#include "CubeModel.h"
#include "CarModel.h"
#include "PlayerControlledCarModel.h"
#include "CylinderModel.h"
#include "GridModel.h"
#include "AxisModel.h"
#include "EventManager.h"

#include <GLFW/glfw3.h>
#include <math.h>
#include <algorithm>


using namespace std;
using namespace glm;

World* World::instance;

World::World()
{
	instance = this;

	// Setup Camera
	FirstPersonCamera* freeCam = new FirstPersonCamera(vec3(3.0f, 5.0f, 20.0f));
	freeCam->SetHorizontalAngle(90.0f);
	mCameras.push_back(freeCam);
	mCurrentCamera = CameraType::CAMERA_FREE;
}

World::~World()
{
	// Static Models
	for (vector<Model*>::iterator it = staticModels.begin(); it < staticModels.end(); ++it)
	{
		delete* it;
	}
	staticModels.clear();

	// Mobile Models
	for (vector<Model*>::iterator it = mobileModels.begin(); it < mobileModels.end(); ++it)
	{
		delete* it;
	}

	mobileModels.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCameras.begin(); it < mCameras.end(); ++it)
	{
		delete* it;
	}
	mCameras.clear();

	// Lights
	for (vector<PointLight*>::iterator it = mPointLights.begin(); it < mPointLights.end(); ++it)
	{
		delete* it;
	}
	mPointLights.clear();

	for (vector<SpotLight*>::iterator it = mSpotLights.begin(); it < mSpotLights.end(); ++it)
	{
		delete* it;
	}
	mSpotLights.clear();

	delete(playerCar);
}

World* World::GetInstance()
{
	return instance;
}

void World::Update(float dt)
{
	GLFWwindow* window = EventManager::GetWindow();
	// User Inputs

	// Switch Camera
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && EventManager::CanUseKey(GLFW_KEY_C)) {
		ToggleCamera();
	}

	// Update current Camera
	if (mCurrentCamera == CameraType::CAMERA_FREE) {
		mCameras[mCurrentCamera]->Update(dt);
	}

	// Update models
	for (vector<Model*>::iterator it = mobileModels.begin(); it < mobileModels.end(); ++it)
	{
		(*it)->Update(dt);
	}

	// Car Updates
	
	// W, A, S, D - Directional Movement
	float carSpeed = 5.0f;
	float carMovement = carSpeed * dt;
	float distanceShift = 0.0f;
	bool isMoving = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		distanceShift += carMovement;
		isMoving = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		distanceShift -= carMovement;
		isMoving = true;
	}
	playerCar->Shift(distanceShift);
	playerCar->SetIsMoving(isMoving);

	float turnSpeed = 60.0f;
	float turnAmount = turnSpeed * dt;
	float turnShift = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		turnShift += turnAmount;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		turnShift -= turnAmount;
	}
	playerCar->Turn(turnShift);

	// P, L, T - Change draw modes for the car
	GLenum drawMode = playerCar->GetDrawMode();
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && EventManager::CanUseKey(GLFW_KEY_P)) {
		drawMode = GL_POINTS;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && EventManager::CanUseKey(GLFW_KEY_L)) {
		drawMode = GL_LINES;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && EventManager::CanUseKey(GLFW_KEY_T)) {
		drawMode = GL_TRIANGLES;
	}
	playerCar->SetDrawMode(drawMode);

	// HOME - Reset car
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS && EventManager::CanUseKey(GLFW_KEY_HOME)) {
		playerCar->Reset();
	}

	// X - Toggle between SolidColor and Textured shaders
	// Default (if any other shader is in use) - Turn on Textured shader
	ShaderType defaultShader = ShaderType::SHADER_SOLID_COLOR;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && EventManager::CanUseKey(GLFW_KEY_X)) {
		ShaderType shaderToggle = ShaderType::SHADER_TEXTURED;
		ShaderType nextShaderToggle = ShaderType::SHADER_TEXTURED_UNCOLORED;
		ShaderType currentShader = ShaderType(Renderer::GetCurrentShader());
		grid->Hide();
		ground->Show();
		if (currentShader == shaderToggle) {
			shaderToggle = nextShaderToggle;
		}
		else if (currentShader == nextShaderToggle) {
			shaderToggle = defaultShader;
			grid->Show();
			ground->Hide();
		}

		Renderer::SetShader(shaderToggle);
	}

	// B - Toggle between SolidColor and Lighting shaders
	// Default (if any other shader is in use) - Turn on Lighting shader
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && EventManager::CanUseKey(GLFW_KEY_B)) {
		ShaderType shaderToggle = ShaderType::SHADER_LIGHTING;
		ShaderType nextShaderToggle = ShaderType::SHADER_LIGHTING_TEXTURED;
		ShaderType currentShader = ShaderType(Renderer::GetCurrentShader());
		grid->Hide();
		ground->Show();
		if (currentShader == shaderToggle) {
			shaderToggle = nextShaderToggle;
		}
		else if (currentShader == nextShaderToggle) {
			shaderToggle = defaultShader;
			grid->Show();
			ground->Hide();
		}

		Renderer::SetShader(shaderToggle);
	}

	// SPACE - Draw car  randomly in a 40 block radius around the center
	// Could be 50, but I didn't like it when it spawned on the edge
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && EventManager::CanUseKey(GLFW_KEY_SPACE)) {
		float randX = -40 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (80)));
		float randZ = -40 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (80)));
		vec3 carPosition = vec3(randX, 0.25f, randZ);
		playerCar->SetCenterPosition(carPosition);
	}

	// U, J - Scale Up/Down car respectively
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		playerCar->UpScale();
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		playerCar->DownScale();
	}

	// Q, E - Rotate car along x-axis
	vec3 carRotation = playerCar->GetRotation();
	const float angleStepSize = 5.0f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		carRotation += vec3(0.0f, 1.0f, 0.0f) * angleStepSize;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		carRotation -= vec3(0.0f, 1.0f, 0.0f) * angleStepSize;
	}

	// R, G - Rotate car along y-axis
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		carRotation += vec3(0.0f, 0.0f, 1.0f) * angleStepSize;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		carRotation -= vec3(0.0f, 0.0f, 1.0f) * angleStepSize;
	}

	// F, H - Rotate car along z-axis
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		carRotation += vec3(1.0f, 0.0f, 0.0f) * angleStepSize;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		carRotation -= vec3(1.0f, 0.0f, 0.0f) * angleStepSize;
	}
	playerCar->SetRotation(carRotation);
	
	playerCar->Update(dt);

	// Projectile Cars
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && mCurrentCamera == CameraType::CAMERA_FREE && EventManager::CanUseKey(GLFW_KEY_5)) {
		const float projectileSpeed = 5.0f;

		vec3 lookAt = ((FirstPersonCamera*) GetCurrentCamera())->GetLookAt();
		vec3 sizeScale(0.1f, 0.1f, 0.1f);
		vec3 pos = ((FirstPersonCamera*)GetCurrentCamera())->GetPosition();
		CarModel* projCar = new CarModel();
		projCar->SetRotation(vec3(0.0f, ((FirstPersonCamera*)GetCurrentCamera())->GetHorizontalAngle(), ((FirstPersonCamera*)GetCurrentCamera())->GetVerticalAngle()));
		projCar->SetCenterPosition(pos);
		projCar->SetVelocity(lookAt * projectileSpeed);
		projCar->SetSizeScale(sizeScale);
		projCar->SetIsMoving(true); // to spin wheels :^)
		projCar->GenerateModel();

		if (projectileCars.size() > 5) {
			projectileCars.pop_front();
		}
		projectileCars.push_back(projCar);
	}

	for (list<CarModel*>::iterator it = projectileCars.begin(); it != projectileCars.end(); ++it) {
		(*it)->Update(dt);
	}
}

void World::Draw()
{
	Renderer::BeginFrame();

	Renderer::BindUniforms();

	// Draw models
	for (vector<Model*>::iterator it = mobileModels.begin(); it < mobileModels.end(); ++it)
	{
		(*it)->Draw();
	}

	for (vector<Model*>::iterator it = staticModels.begin(); it < staticModels.end(); ++it)
	{
		(*it)->Draw();
	}

	playerCar->Draw();

	for (list<CarModel*>::iterator it = projectileCars.begin(); it != projectileCars.end(); ++it) {
		(*it)->Draw();
	}

	// Lights
	for (vector<SpotLight*>::iterator it = mSpotLights.begin(); it != mSpotLights.end(); ++it) {
		(*it)->GetLightModel()->Draw();
	}
	for (vector<PointLight*>::iterator it = mPointLights.begin(); it != mPointLights.end(); ++it) {
		(*it)->GetLightModel()->Draw();
	}

	Renderer::EndFrame();
}

const Camera* World::GetCurrentCamera() const
{
	return mCameras[mCurrentCamera];
}

const CameraType World::GetCurrentCameraType() const {
	return CameraType(mCurrentCamera);
}

void World::InitializeModels() {
	vec3 darkSlateGray(47, 79, 79);
	vec3 silver(192, 192, 192);
	vec3 gold(255, 215, 0);
	vec3 red(1.0f, 0.0f, 0.0f);

	vec3 centerPos(0.0f, 30.0f, 0.0f);
	vec3 color(1.0f, 1.0f, 1.0f);
	CubeModel* lightCube = new CubeModel();
	lightCube->SetSizeScale(vec3(2.0f, 2.0f, 2.0f));
	lightCube->SetCenterPosition(centerPos);
	PointLight* pointLight = new PointLight(centerPos, color);
	pointLight->SetLightModel(lightCube);
	pointLight->GetLightModel()->GenerateModel();
	pointLight->SetLinear(0.0f);
	pointLight->SetQuadratic(0.0f);
	mPointLights.push_back(pointLight);

	playerCar = new PlayerControlledCarModel(vec3(0.0f, 0.75f, 0.0f));
	playerCar->SetColorFromVec3(gold);
	playerCar->GenerateModel();

	grid = new GridModel(vec3(0.0f, 0.0f, 0.0f));
	grid->GenerateModel();
	staticModels.push_back(grid);

	ground = new CubeModel(vec3(0.0f, -0.25f, 0.0f));
	ground->SetSizeScale(vec3(100.0f, 0.5f, 100.0f));
	ground->SetTexture(TextureType::TEXTURE_GRASS);
	ground->GenerateModel();
	ground->Hide();
	staticModels.push_back(ground);

	AxisModel* axis = new AxisModel(vec3(0.0f, 0.0f, 0.0f));
	axis->GenerateModel();
	staticModels.push_back(axis);
}

mat4 World::GetInstancedViewProjectionMatrix() {
	return instance->GetCurrentCamera()->GetViewProjectionMatrix();
}

void World::AddSpotLight(SpotLight* light) {
	mSpotLights.push_back(light);
}

void World::RemoveSpotLight(SpotLight* light) {
	mSpotLights.erase(std::remove(mSpotLights.begin(), mSpotLights.end(), light), mSpotLights.end());
}

void World::AddPointLight(PointLight* light) {
	mPointLights.push_back(light);
}

void World::RemovePointLight(PointLight* light) {
	mPointLights.erase(std::remove(mPointLights.begin(), mPointLights.end(), light), mPointLights.end());
}

void World::AddCamera(CameraType type, Camera* camera) {
	if (type < mCameras.size()) {
		mCameras[type] = camera;
	}
	else {
		mCameras.push_back(camera);
	}
}

void World::ToggleCamera() {
	int modulo = mCameras.size();
	mCurrentCamera = (mCurrentCamera + 1) % modulo;
}