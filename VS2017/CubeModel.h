#pragma once

#include "Model.h"

class CubeModel : public Model {
public:
	CubeModel(
		glm::vec3 centerPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 centerShift = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 sizeScale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 shapeScale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f)
	);
	virtual ~CubeModel();

	virtual void Update(float dt);
	virtual void Draw();
	virtual void GenerateModel();

private:
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 aUV;
	//	float 
	};

	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
};