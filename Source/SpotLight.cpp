#include "SpotLight.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <math.h>

using namespace std;
using namespace glm;

SpotLight::SpotLight(
	glm::vec3 centerPosition,
	glm::vec3 direction,
	glm::vec3 color,
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	Model* lightModel
): mDirection(direction), PointLight(centerPosition, color, ambient, diffuse, specular, lightModel) {
	mCutOff = glm::cos(glm::radians(25.5f));
	mOuterCutOff = glm::cos(glm::radians(35.5f));
}

SpotLight::~SpotLight() {
}

void SpotLight::SetDirection(glm::vec3 direction) {
	mDirection = direction;
}

void SpotLight::SetCutOff(float cutOff) {
	mCutOff = cutOff;
}

void SpotLight::SetOuterCutOff(float outerCutOff) {
	mOuterCutOff = outerCutOff;
}

void SpotLight::UpdateFromModel() {
	vec3 newPos = mLightModel->GetCenterPosition() + mLightModel->GetCenterShift();

	mat4 centerT = translate(mat4(1.0f), mLightModel->GetCenterPosition());

	mat4 rot = Model::ComputeRotationMatrix(mLightModel->GetRotation());

	mat4 shift = translate(mat4(1.0f), mLightModel->GetCenterShift());
	
	mat4 translated = centerT * rot * shift;

	mPosition = vec3(translated[3]);
}