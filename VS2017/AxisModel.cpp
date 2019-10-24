#include "AxisModel.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

AxisModel::AxisModel(
) {
	SetDrawMode(GL_LINES);
	vec3 red = ComputeColorFromRGB(255, 0, 0);
	vec3 green = ComputeColorFromRGB(0, 255, 0);
	vec3 blue = ComputeColorFromRGB(0, 0, 255);

	vec3 vertexBuffer[] = {
		vec3(0.0f, 0.0f, 0.0f),		red,			// Line along x, red				8-9
		vec3(1.0f, 0.0f, 0.0f),		red,

		vec3(0.0f, 0.0f, 0.0f),		green,			// Line along y, green				10-11
		vec3(0.0f, 1.0f, 0.0f),		green,

		vec3(0.0f, 0.0f, 0.0f),		blue,			// Line along z, blue				12-13
		vec3(0.0f, 0.0f, 1.0f),		blue,
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

	// 2nd attribute buffer : vertex color
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(vec3),
		(void*) sizeof(vec3)
	);
	glEnableVertexAttribArray(2);
}

AxisModel::~AxisModel() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void AxisModel::Update(float dt) {
	Model::Update(dt);
}

void AxisModel::Draw()
{
	glBindVertexArray(mVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");

	mat4 axeWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &axeWorldMatrix[0][0]);
	glDrawArrays(GL_LINES, 0, 6); 
}