#include "CylinderModel.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

CylinderModel::CylinderModel(
	vec3 centerPosition, vec3 centerShift,
	vec3 sizeScale, vec3 shapeScale,
	vec3 rotation, vec3 color,
	TextureType texture, TextureType circleTexture
) {
	mCenterPosition = centerPosition;
	mCenterShift = centerShift;
	mSizeScale = sizeScale;
	mShapeScale = shapeScale;
	mRotation = rotation;
	mColor = color;
	mTexture = texture;
	mCircleTexture = circleTexture;
}

CylinderModel::~CylinderModel() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void CylinderModel::GenerateModel() {
	GenerateCylinder();

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mCylinderPoints.size(), &(mCylinderPoints)[0], GL_STATIC_DRAW);



	// 1st attribute buffer : vertex Positions
	glVertexAttribPointer(0,                // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                // size
		GL_FLOAT,        // type
		GL_FALSE,        // normalized?
		sizeof(Vertex), // stride
		(void*)0        // array buffer offset
	);
	glEnableVertexAttribArray(0);

	// 2nd attribute buffer : vertex normal
	glVertexAttribPointer(1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
	);
	glEnableVertexAttribArray(1);

	// 3rd attribute buffer : vertex color
	glVertexAttribPointer(2,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
	);
	glEnableVertexAttribArray(2);

	// 4th attribute buffer : aUV
	glVertexAttribPointer(3,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(3 * sizeof(vec3)) // uAV is Offseted by 3 vec3 (see class Vertex)
	);
	glEnableVertexAttribArray(3);
}

void CylinderModel::GenerateCylinder(){
	GenerateCircles();
	GenerateWall();
}

void CylinderModel::GenerateCircles() {
	mNegCircle.clear();
	mPosCircle.clear();

	vec3 circleCenters[2] = { vec3(0.0f, 0.5f, -0.5f), vec3(0.0f, 0.5f, 0.5f) };
	vec3 negNormal(0.0f, 0.0f, -1.0f);
	vec3 posNormal(0.0f, 0.0f, 1.0f);
	float radius = 0.5f;
	int numSplines = 64; // want this to be divisible by 4
	float angleStep = 360.0f / numSplines;

	for (int i = 0; i < 2; i++) {
		float angle = 0.0f;
		vec3 circleCenter = circleCenters[i];

		// I mean it's only 2 iterations so I really don't need this but whatever...
		bool isNegCircle = circleCenter.z < 0;
		if (isNegCircle) {
			mNegCircleCenter = circleCenter;
		}
		else {
			mPosCircleCenter = circleCenter;
		}

		for (int j = 0; j < numSplines; j++) {
			angle = j * angleStep;
			vec3 position1 = GeneratePositionOnCircle(radius, angle, circleCenter);
			vec3 position2 = GeneratePositionOnCircle(radius, angle + angleStep, circleCenter);
			vec3 normal = isNegCircle ? negNormal : posNormal;

			vec2 avTopLeft(0.0f, 1.0f);
			vec2 avTopRight(1.0f, 1.0f);
			vec2 avCenter(0.5f, 0.0f);

			Vertex point1 = { position1, normal, mColor, avTopLeft };
			Vertex point2 = { position2, normal, mColor, avTopRight };
			Vertex center = { circleCenter, normal, mColor, avCenter };

			if (isNegCircle) {
				mCylinderPoints.push_back(point1);
				int lastIndex = mCylinderPoints.size() - 1;
				mNegCircle.push_back(mCylinderPoints[lastIndex]);

				mCylinderPoints.push_back(point2);
				lastIndex = mCylinderPoints.size() - 1;
				mNegCircle.push_back(mCylinderPoints[lastIndex]);

				mCylinderPoints.push_back(center);
				lastIndex = mCylinderPoints.size() - 1;
				mNegCircle.push_back(mCylinderPoints[lastIndex]);
			} else {
				mCylinderPoints.push_back(point1);
				int lastIndex = mCylinderPoints.size() - 1;
				mPosCircle.push_back(mCylinderPoints[lastIndex]);

				mCylinderPoints.push_back(center);
				lastIndex = mCylinderPoints.size() - 1;
				mPosCircle.push_back(mCylinderPoints[lastIndex]);

				mCylinderPoints.push_back(point2);
				lastIndex = mCylinderPoints.size() - 1;
				mPosCircle.push_back(mCylinderPoints[lastIndex]);
			}
		}
	}
}

vec3 CylinderModel::GeneratePositionOnCircle(float radius, float angle, vec3 center) {
	float theta = radians(angle);
	float dx = radius * cos(theta);
	float dy = radius * sin(theta);

	return vec3(center.x + dx, center.y + dy, center.z);
}

void CylinderModel::GenerateWall() {
	int numPos = mPosCircle.size();
	float chunkOfTexture = 1.0f / (numPos / 3); // repeat texture for every quarter of the wheel
	float texturePosition = 0.0f;
	for (int i = 0; i < numPos; i = i + 3) {
		float nextTexturePosition = texturePosition + chunkOfTexture;
		int nextIndex = (i + 3) % numPos;
		// triangle points
		vec3 brP = mPosCircle[i].position;
		vec3 brN = normalize(brP - mPosCircleCenter);
		vec2 brUV(1.0f, texturePosition);
		Vertex bottomRight = { brP, brN, mColor, brUV };

		vec3 trP = mPosCircle[nextIndex].position;
		vec3 trN = normalize(trP - mPosCircleCenter);
		vec2 trUV(1.0f, nextTexturePosition);
		Vertex topRight = { trP, trN, mColor, trUV };

		vec3 blP = mNegCircle[i].position;
		vec3 blN = normalize(blP - mNegCircleCenter);
		vec2 blUV(0.0f, texturePosition);
		Vertex bottomLeft = { blP, blN, mColor, blUV };

		vec3 tlP = mNegCircle[nextIndex].position;
		vec3 tlN = normalize(tlP - mNegCircleCenter);
		vec2 tlUV(0.0f, nextTexturePosition);
		Vertex topLeft = { tlP, tlN, mColor, tlUV };

		// t1 - bottomright - topright - bottomleft
 		mCylinderPoints.push_back(bottomRight);
		mWall.push_back(bottomRight);
		mCylinderPoints.push_back(topRight);
		mWall.push_back(topRight);
		mCylinderPoints.push_back(bottomLeft);
		mWall.push_back(bottomLeft);

		// t2 - bottomleft - topright - topleft
		mCylinderPoints.push_back(bottomLeft);
		mWall.push_back(bottomLeft);
		mCylinderPoints.push_back(topRight);
		mWall.push_back(topRight);
		mCylinderPoints.push_back(topLeft);
		mWall.push_back(topLeft);

		texturePosition = fmod(texturePosition + chunkOfTexture, 1.0f);
	}
}

void CylinderModel::Update(float dt) {
	Model::Update(dt);
}

void CylinderModel::Draw()
{
	if (mHidden) {
		return;
	}

	int circlesIndex = 0;
	int circlesCount = mNegCircle.size() + mPosCircle.size();
	int wallIndex = circlesIndex + circlesCount;
	int wallCount = mWall.size();

	glBindVertexArray(mVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);


	// Draw circles 
	if (Renderer::ShaderNeedsTexture()) {
		glBindTexture(GL_TEXTURE_2D, Renderer::GetTextureID(mCircleTexture));
	}
	Renderer::BindTextureUniforms(mCircleTexture);
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &(GetWorldMatrix())[0][0]);
	glDrawArrays(mDrawMode, circlesIndex, circlesCount);


	// Draw wall 
	if (Renderer::ShaderNeedsTexture()) {
		glBindTexture(GL_TEXTURE_2D, Renderer::GetTextureID(mTexture));
	}
	Renderer::BindTextureUniforms(mTexture);
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &(GetWorldMatrix())[0][0]);
	glDrawArrays(mDrawMode, wallIndex, wallCount);
}