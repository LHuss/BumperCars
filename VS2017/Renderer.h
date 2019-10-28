#pragma once

struct GLFWwindow;

#include <string>

// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC 1

#include <GL/glew.h>

#include <vector>
#include <glm/glm.hpp>


enum ShaderType
{
	SHADER_SOLID_COLOR,
	SHADER_TEXTURED,
	SHADER_TEXTURED_UNCOLORED,
	SHADER_LIGHTING,
	SHADER_LIGHTING_TEXTURED,
	SHADER_LIGHTING_TEXTURED_TWOPASS,
	NUM_SHADERS,
	SHADER_NULL
};

enum TextureType
{
	TEXTURE_BRICK,
	TEXTURE_CEMENT,
	TEXTURE_GRASS,
	TEXTURE_STEEL,
	TEXTURE_TIRE,
	TEXTURE_WOOD,
	NUM_TEXTURES,
	TEXTURE_NULL
};

struct Material {
	glm::vec3 diffuse;
	float specular;
	float shininess;
};

class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();

	static GLuint LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path);

	static GLuint LoadTexture(std::string texture_path_s);

	static unsigned int GetShaderProgramID() { return sShaderProgramID[sCurrentShader]; }
	static unsigned int GetCurrentShader() { return sCurrentShader; }
	static void SetShader(ShaderType type);

	static unsigned int GetTextureID(TextureType texture);
	static float GetShininess(TextureType texture);
	static float GetAmbientStrength(TextureType texture);
	static float GetSpecularStrength(TextureType texture);

	static void CheckForErrors();
	static bool PrintError();

	static bool ShaderNeedsTexture();

	static void SwapAndUseShader(ShaderType type);

	static void BindUniforms();
	static void BindTextureUniforms(TextureType texture);

private:
	static GLFWwindow* spWindow;

	static std::vector<unsigned int> sShaderProgramID;
	static unsigned int sCurrentShader;

	static std::vector<unsigned int> sTextureID;

};

