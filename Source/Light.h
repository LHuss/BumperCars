#pragma once

#include "Model.h"

class Light {
public:
	Light(
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 ambient = glm::vec3(0.9f, 0.9f, 0.9f),
		glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f)
	);
	virtual ~Light();

	void SetColor(glm::vec3 color);
	void SetAmbient(glm::vec3 ambient);
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);

	glm::vec3 GetColor() const { return mColor; }
	glm::vec3 GetAmbient() const { return mAmbient; }
	glm::vec3 GetDiffuse() const { return mDiffuse; }
	glm::vec3 GetSpecular() const { return mSpecular; }

protected:
	glm::vec3 mColor;

	glm::vec3 mAmbient;
	glm::vec3 mDiffuse;
	glm::vec3 mSpecular;
};