#include "PointLight.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <math.h>

using namespace std;
using namespace glm;

PointLight::PointLight(
	glm::vec3 centerPosition,
	glm::vec3 color,
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	Model* lightModel
): mPosition(centerPosition), Light(color, ambient, diffuse, specular) {
	mLightModel = lightModel;
	AdjustLightModel();
}

PointLight::~PointLight() {
	delete mLightModel;
}

void PointLight::SetPosition(glm::vec3 position) {
	mPosition = position;
	mLightModel->SetCenterPosition(position);
}

void PointLight::SetConstant(float constant) {
	mConstant = constant;
}

void PointLight::SetLinear(float linear) {
	mLinear = linear;
}

void PointLight::SetQuadratic(float quadratic) {
	mQuadratic = quadratic;
}

void PointLight::SetLightModel(Model* lightModel) {
	mLightModel = lightModel;
	AdjustLightModel();
}

void PointLight::AdjustLightModel() {
	mLightModel->SetCenterPosition(mPosition);
	mLightModel->SetColor(mColor);
	mLightModel->SetSpecificShader(ShaderType::SHADER_SOLID_COLOR);
}