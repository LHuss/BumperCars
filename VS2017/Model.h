#pragma once


#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

class Model {
public:
	Model();
	virtual ~Model();

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	
	virtual glm::mat4 GetWorldMatrix() const;

	void SetCenterPosition(glm::vec3 position);
	void SetCenterShift(glm::vec3 shift);
	void SetShapeScale(glm::vec3 shape);
	void SetSizeScale(glm::vec3 size);
	void SetRotation(glm::vec3 rotation);
	void SetColor(glm::vec3 color);
	void SetColorFromRGB(int r, int g, int b);
	void SetDrawMode(GLenum drawMode);

	void UpScale();
	void DownScale();

	glm::vec3 GetCenterPosition() const { return mCenterPosition; }
	glm::vec3 GetCenterShift() const { return mCenterShift; }
	glm::vec3 GetSizeScale() const { return mSizeScale; }
	glm::vec3 GetShapeScale() const { return mShapeScale; }
	glm::vec3 GetRotation() const { return mRotation; }
	glm::vec3 GetColor() const { return mColor; }
	GLenum GetDrawMode() const { return mDrawMode; }

	glm::vec3 ComputeColorFromRGB(int r, int g, int b);

protected:
	glm::vec3 mCenterPosition;
	glm::vec3 mCenterShift;
	glm::vec3 mSizeScale;
	glm::vec3 mShapeScale;
	glm::vec3 mRotation;
	glm::vec3 mColor;

	// Purely for projectile shits & giggles
	glm::vec3 mRotationVelocity;
	
	GLenum mDrawMode;

	float mAngleStepSize = 5.0f;
};
