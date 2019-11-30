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