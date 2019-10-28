#include "GridModel.h"
#include "Renderer.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

GridModel::GridModel(vec3 position) {
	mCenterPosition = position;
	mSpecificShader = ShaderType::SHADER_SOLID_COLOR;
}

GridModel::~GridModel() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void GridModel::GenerateModel() {
	// light blue
	vec3 lightBlue = ComputeColorFromRGB(68, 85, 90);

	SetDrawMode(GL_LINES);

	vec3 vertexBuffer[] = {
		vec3(0.0f, 0.0f, 0.0f),		lightBlue,		// Light blue square - for grid		
		vec3(0.0f, 0.0f, 1.0f),		lightBlue,

		vec3(0.0f, 0.0f, 1.0f),		lightBlue,
		vec3(1.0f, 0.0f, 1.0f),		lightBlue,

		vec3(1.0f, 0.0f, 1.0f),		lightBlue,
		vec3(1.0f, 0.0f, 0.0f),		lightBlue,

		vec3(1.0f, 0.0f, 0.0f),		lightBlue,
		vec3(0.0f, 0.0f, 0.0f),		lightBlue,
	};

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);



	// 1st attribute buffer : vertex Positions
	glVertexAttribPointer(0,                // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                // size
		GL_FLOAT,        // type
		GL_FALSE,        // normalized?
		2 * sizeof(vec3),   // stride
		(void*)0        // array buffer offset
	);
	glEnableVertexAttribArray(0);

	// 3rd attribute buffer : vertex color
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(vec3),
		(void*) sizeof(vec3)
	);
	glEnableVertexAttribArray(2);
}

void GridModel::Update(float dt) {
	Model::Update(dt);
}

void GridModel::Draw()
{
	if (mHidden) {
		return;
	}

	// Save current shader to swap back
	ShaderType programShader = ShaderType(Renderer::GetCurrentShader());
	// Swap shaders
	Renderer::SwapAndUseShader(mSpecificShader);


	// Draw
	glBindVertexArray(mVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");

	for (int i = -50; i < 50; i++) {
		for (int j = -50; j < 50; j++) {
			mat4 gridWorldMatrix = translate(mat4(1.0f), vec3(mCenterPosition.x + i, mCenterPosition.y, mCenterPosition.z + j));
			glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
			glDrawArrays(GL_LINES, 0, 8);
		}
	}

	// Swap Back
	Renderer::SwapAndUseShader(programShader);
}