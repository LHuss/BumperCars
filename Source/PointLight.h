#pragma once

#include "Light.h"
#include "Model.h"
#include "CubeModel.h"

class PointLight : public Light {
public:
	PointLight(
		glm::vec3 centerPosition = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 ambient = glm::vec3(0.9f, 0.9f, 0.9f),
		glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f),
		Model* lightModel = new CubeModel()
	);
	virtual ~PointLight();

	void SetPosition(glm::vec3 position);
	void SetConstant(float constant);
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);
	void SetLightModel(Model* lightModel);

	glm::vec3 GetPosition() const { return mPosition; }
	float GetConstant() const { return mConstant; }
	float GetLinear() const { return mLinear; }
	float GetQuadratic() const { return mQuadratic; }
	Model* GetLightModel() const { return mLightModel; }

protected:
	glm::vec3 mPosition;

	float mConstant;
	float mLinear;
	float mQuadratic;

	Model* mLightModel;

	void AdjustLightModel();
};