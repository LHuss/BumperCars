#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <math.h>

using namespace std;
using namespace glm;

Light::Light(vec3 color, vec3 ambient, vec3 diffuse, vec3 specular) :
	mColor(color),
	mAmbient(ambient), 
	mDiffuse(diffuse), 
	mSpecular(specular), 
	mActive(true) {
}

Light::~Light() {
}

void Light::Toggle() {
	mActive = !mActive;
}

void Light::Enable() {
	mActive = true;
}

void Light::Disable() {
	mActive = false;
}

void Light::SetColor(vec3 color) {
	mColor = color;
}

void Light::SetAmbient(vec3 ambient) {
	mAmbient = ambient;
}

void Light::SetDiffuse(vec3 diffuse) {
	mDiffuse = diffuse;
}

void Light::SetSpecular(vec3 specular) {
	mSpecular = specular;
}