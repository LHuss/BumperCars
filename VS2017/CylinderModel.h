#pragma once

#include "Model.h"

class CylinderModel : public Model {
public:
	CylinderModel(
		glm::vec3 centerPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 centerShift = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 sizeScale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 shapeScale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f),
		TextureType texture = TextureType::TEXTURE_TIRE, TextureType circleTexture = TextureType::TEXTURE_STEEL
	);
	virtual ~CylinderModel();

	void GenerateCylinder();
	void GenerateCircles();
	void GenerateWall();

	virtual void Update(float dt);
	virtual void Draw();

private:
	glm::vec3 GeneratePositionOnCircle(float radius, float angle, glm::vec3 center);

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 aUV;
	};

	unsigned int mVertexArray;
	unsigned int mVertexBuffer;
	
	std::vector<Vertex> mWall;
	std::vector<Vertex> mNegCircle;
	std::vector<Vertex> mPosCircle;

	std::vector<Vertex> mCylinderPoints;

	TextureType mCircleTexture;
};