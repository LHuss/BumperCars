#pragma once

#include "Model.h"

class GridModel : public Model {
public:
	GridModel(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~GridModel();

	virtual void Update(float dt);
	virtual void Draw();
	virtual void GenerateModel();

private:

	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
};