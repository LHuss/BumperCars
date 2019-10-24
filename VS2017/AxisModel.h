#pragma once

#include "Model.h"

class AxisModel : public Model {
public:
	AxisModel();
	virtual ~AxisModel();

	virtual void Update(float dt);
	virtual void Draw();

private:

	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
};