#pragma once

#include "Model.h"

class GridModel : public Model {
public:
	GridModel();
	virtual ~GridModel();

	virtual void Update(float dt);
	virtual void Draw();

private:

	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
};