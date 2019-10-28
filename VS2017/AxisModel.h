#pragma once

#include "Model.h"

class AxisModel : public Model {
public:
	AxisModel(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~AxisModel();

	virtual void Update(float dt);
	virtual void Draw();
	virtual void GenerateModel();

private:

	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
};