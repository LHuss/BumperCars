#include "CubeModel.h"
#include "Renderer.h"

using namespace glm;

CubeModel::CubeModel(
	vec3 centerPosition, vec3 centerShift,
	vec3 sizeScale, vec3 shapeScale,
	vec3 rotation, vec3 color
) {
	mCenterPosition = centerPosition;
	mCenterShift = centerShift;
	mSizeScale = sizeScale;
	mShapeScale = shapeScale;
	mRotation = rotation;
	mColor = color;
	mMax = vec3(0.5, 0.5f, 0.5f);
	mMin = vec3(-0.5f, -0.5f, -0.5f);
}

CubeModel::~CubeModel() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void CubeModel::GenerateModel() {
	float lowerBound = -0.5f;
	float upperBound = 0.5f;

	Vertex vertexBuffer[] = {  // position,									normal,						color,  aUV,				ambient		specular
								{ vec3(mMin.x, mMin.y, mMin.z),	vec3(-1.0f, 0.0f, 0.0f),	mColor, vec2(1.0f, 0.0f)	 }, // left
								{ vec3(mMin.x, mMin.y, mMax.z),	vec3(-1.0f, 0.0f, 0.0f),	mColor, vec2(0.0f, 0.0f) },
								{ vec3(mMin.x, mMax.y, mMax.z),	vec3(-1.0f, 0.0f, 0.0f),	mColor, vec2(0.0f, 1.0f) },

								{ vec3(mMin.x, mMin.y, mMin.z),	vec3(-1.0f, 0.0f, 0.0f),	mColor, vec2(1.0f, 0.0f) },
								{ vec3(mMin.x, mMax.y, mMax.z),	vec3(-1.0f, 0.0f, 0.0f),	mColor, vec2(0.0f, 1.0f) },
								{ vec3(mMin.x, mMax.y, mMin.z),	vec3(-1.0f, 0.0f, 0.0f),	mColor, vec2(1.0f, 1.0f) },

								{ vec3(mMax.x, mMax.y, mMin.z),	vec3(0.0f, 0.0f,-1.0f),		mColor, vec2(1.0f, 1.0f) }, // far
								{ vec3(mMin.x, mMin.y, mMin.z),	vec3(0.0f, 0.0f,-1.0f),		mColor, vec2(0.0f, 0.0f) },
								{ vec3(mMin.x, mMax.y, mMin.z),	vec3(0.0f, 0.0f,-1.0f),		mColor, vec2(0.0f, 1.0f) },

								{ vec3(mMax.x, mMax.y, mMin.z),	vec3(0.0f, 0.0f,-1.0f),		mColor, vec2(1.0f, 1.0f) },
								{ vec3(mMax.x, mMin.y, mMin.z),	vec3(0.0f, 0.0f,-1.0f),		mColor, vec2(1.0f, 0.0f) },
								{ vec3(mMin.x, mMin.y, mMin.z),	vec3(0.0f, 0.0f,-1.0f),		mColor, vec2(0.0f, 0.0f) },

								{ vec3(mMax.x, mMin.y, mMax.z),	vec3(0.0f,-1.0f, 0.0f),		mColor, vec2(1.0f, 1.0f) }, // bottom
								{ vec3(mMin.x, mMin.y, mMin.z),	vec3(0.0f,-1.0f, 0.0f),		mColor, vec2(0.0f, 0.0f) },
								{ vec3(mMax.x, mMin.y, mMin.z),	vec3(0.0f,-1.0f, 0.0f),		mColor, vec2(1.0f, 0.0f) },

								{ vec3(mMax.x, mMin.y, mMax.z),	vec3(0.0f,-1.0f, 0.0f),		mColor, vec2(1.0f, 1.0f) },
								{ vec3(mMin.x, mMin.y, mMax.z),	vec3(0.0f,-1.0f, 0.0f),		mColor, vec2(.0f, 1.0f) },
								{ vec3(mMin.x, mMin.y, mMin.z),	vec3(0.0f,-1.0f, 0.0f),		mColor, vec2(0.0f, 0.0f) },

								{ vec3(mMin.x, mMax.y, mMax.z),	vec3(0.0f, 0.0f, 1.0f),		mColor, vec2(1.0f, 1.0f) }, // near
								{ vec3(mMin.x, mMin.y, mMax.z),	vec3(0.0f, 0.0f, 1.0f),		mColor, vec2(1.0f, 0.0f) },
								{ vec3(mMax.x, mMin.y, mMax.z),	vec3(0.0f, 0.0f, 1.0f),		mColor, vec2(0.0f, 0.0f) },

								{ vec3(mMax.x, mMax.y, mMax.z),	vec3(0.0f, 0.0f, 1.0f),		mColor, vec2(0.0f, 1.0f) },
								{ vec3(mMin.x, mMax.y, mMax.z),	vec3(0.0f, 0.0f, 1.0f),		mColor, vec2(1.0f, 1.0f) },
								{ vec3(mMax.x, mMin.y, mMax.z),	vec3(0.0f, 0.0f, 1.0f),		mColor, vec2(0.0f, 0.0f) },

								{ vec3(mMax.x, mMax.y, mMax.z),	vec3(1.0f, 0.0f, 0.0f),		mColor, vec2(1.0f, 1.0f) }, // right
								{ vec3(mMax.x, mMin.y, mMin.z),	vec3(1.0f, 0.0f, 0.0f),		mColor, vec2(0.0f, 0.0f) },
								{ vec3(mMax.x, mMax.y, mMin.z),	vec3(1.0f, 0.0f, 0.0f),		mColor, vec2(0.0f, 1.0f) },

								{ vec3(mMax.x, mMin.y, mMin.z),	vec3(1.0f, 0.0f, 0.0f),		mColor, vec2(0.0f, 0.0f) },
								{ vec3(mMax.x, mMax.y, mMax.z),	vec3(1.0f, 0.0f, 0.0f),		mColor, vec2(1.0f, 1.0f) },
								{ vec3(mMax.x, mMin.y, mMax.z),	vec3(1.0f, 0.0f, 0.0f),		mColor, vec2(1.0f, 0.0f) },

								{ vec3(mMax.x, mMax.y, mMax.z),	vec3(0.0f, 1.0f, 0.0f),		mColor, vec2(0.0f, 0.0f) }, //top
								{ vec3(mMax.x, mMax.y, mMin.z),	vec3(0.0f, 1.0f, 0.0f),		mColor, vec2(0.0f, 1.0f) },
								{ vec3(mMin.x, mMax.y, mMin.z),	vec3(0.0f, 1.0f, 0.0f),		mColor, vec2(1.0f, 1.0f) },

								{ vec3(mMax.x, mMax.y, mMax.z),	vec3(0.0f, 1.0f, 0.0f),		mColor, vec2(1.0f, 0.0f) },
								{ vec3(mMin.x, mMax.y, mMin.z),	vec3(0.0f, 1.0f, 0.0f),		mColor, vec2(0.0f, 1.0f) },
								{ vec3(mMin.x, mMax.y, mMax.z),	vec3(0.0f, 1.0f, 0.0f),		mColor, vec2(0.0f, 0.0f) }
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

void CubeModel::Update(float dt) {
	Model::Update(dt);
}

void CubeModel::Draw()
{
	if (mHidden) {
		return;
	}

	ShaderType programShader = ShaderType(Renderer::GetCurrentShader());
	Renderer::SwapAndUseShader(mSpecificShader);

	if (Renderer::ShaderNeedsTexture()) {
		if (mTexture == TextureType::TEXTURE_NULL) {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, Renderer::GetTextureID(mTexture));
		}
	}

	Renderer::BindTextureUniforms(mTexture);

	glBindVertexArray(mVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	glDrawArrays(mDrawMode, 0, 36);

	Renderer::SwapAndUseShader(programShader);
}

void CubeModel::SetMaxes(
	vec3 max, vec3 min
) {
	mMax = max;
	mMin = min;
}