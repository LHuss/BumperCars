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
	NUM_SHADERS
};

enum TextureType
{
	TEXTURE_BRICK,
	TEXTURE_CEMENT,
	TEXTURE_GRASS,
	TEXTURE_STEEL,
	TEXTURE_TIRE,
	TEXTURE_WOOD,
	NUM_TEXTURES
};


class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();

	static GLuint LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path);

	static GLuint LoadTexture(char* texture_path);

	static unsigned int GetShaderProgramID() { return sShaderProgramID[sCurrentShader]; }
	static unsigned int GetCurrentShader() { return sCurrentShader; }
	static void SetShader(ShaderType type);

	static unsigned int GetTextureID(TextureType texture);

	static void CheckForErrors();
	static bool PrintError();

	static bool ShaderNeedsTexture();

	static void SwapAndUseShader(ShaderType type);

private:
	static GLFWwindow* spWindow;

	static std::vector<unsigned int> sShaderProgramID;
	static unsigned int sCurrentShader;

	static std::vector<unsigned int> sTextureID;

};

