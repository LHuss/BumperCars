#pragma once

#include "PointLight.h"
#include "Model.h"
#include "CubeModel.h"

class SpotLight : public PointLight {
public:
	SpotLight(
		glm::vec3 centerPosition = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3 ambient = glm::vec3(0.9f, 0.9f, 0.9f),
		glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f),
		Model* lightModel = new CubeModel()
	);
	virtual ~SpotLight();

	void SetDirection(glm::vec3 direction);
	void SetCutOff(float cutOff);
	void SetOuterCutOff(float outerCutOff);

	glm::vec3 GetDirection() const { return mDirection; }
	float GetCutOff() const { return mCutOff; }
	float GetOuterCutOff() const { return mOuterCutOff; }

private:
	glm::vec3 mDirection;
	float mCutOff;
	float mOuterCutOff;
};