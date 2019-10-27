//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include "Renderer.h"
#include "EventManager.h"
#include "World.h"

#include <GLFW/glfw3.h>
#include <FreeImageIO.h>


#if defined(PLATFORM_OSX)
#define fscanf_s fscanf
#endif


std::vector<unsigned int> Renderer::sShaderProgramID;
unsigned int Renderer::sCurrentShader;

std::vector<unsigned int> Renderer::sTextureID;

GLFWwindow* Renderer::spWindow = nullptr;


void Renderer::Initialize()
{
	spWindow = EventManager::GetWindow();
	glfwMakeContextCurrent(spWindow);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");

		getchar();
		exit(-1);
	}

	// Somehow, glewInit triggers a glInvalidEnum... Let's ignore it
	glGetError();

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	CheckForErrors();




	// Loading Shaders
#if defined(PLATFORM_OSX)
	std::string shaderPathPrefix = "Shaders/";
#else
	std::string shaderPathPrefix = "../Assets/Shaders/";
#endif

	sShaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "SolidColor.vertexshader",
			shaderPathPrefix + "SolidColor.fragmentshader")
	);
	sShaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "Textured.vertexshader",
			shaderPathPrefix + "Textured.fragmentshader")
	);
	sShaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "Textured_Uncolored.vertexshader",
			shaderPathPrefix + "Textured_Uncolored.fragmentshader")
	);

	sCurrentShader = 0;

	// Loading Textures
#if defined(PLATFORM_OSX)
	std::string texturePathPrefix = "Textures/";
#else
	std::string texturePathPrefix = "../Assets/Textures/";
#endif

	sTextureID.push_back(
		LoadTexture(texturePathPrefix + "brick.jpg")
	);

	sTextureID.push_back(
		LoadTexture(texturePathPrefix + "cement.jpg")
	);

	sTextureID.push_back(
		LoadTexture(texturePathPrefix + "grass.bmp")
	);

	sTextureID.push_back(
		LoadTexture(texturePathPrefix + "steel.jpg")
	);

	sTextureID.push_back(
		LoadTexture(texturePathPrefix + "tire.jpg")
	);

	sTextureID.push_back(
		LoadTexture(texturePathPrefix + "wood.png")
	);

}

void Renderer::Shutdown()
{
	// Shaders
	for (vector<unsigned int>::iterator it = sShaderProgramID.begin(); it < sShaderProgramID.end(); ++it)
	{
		glDeleteProgram(*it);
	}
	sShaderProgramID.clear();


	// Managed by EventManager
	spWindow = nullptr;
}

void Renderer::BeginFrame()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::EndFrame()
{
	// Swap buffers
	glfwSwapBuffers(spWindow);

	CheckForErrors();
}

void Renderer::SetShader(ShaderType type)
{
	if (type < (int)sShaderProgramID.size())
	{
		sCurrentShader = type;
	}
}

void Renderer::SwapAndUseShader(ShaderType type) {
	ShaderType currentShader = ShaderType(GetCurrentShader());
	if (currentShader != type) {
		SetShader(type);

		glUseProgram(GetShaderProgramID());

		// Update ViewProjection of new shader program
		glm::mat4 VP = World::GetInstancedViewProjectionMatrix();
		GLuint VPMatrixLocation = glGetUniformLocation(GetShaderProgramID(), "ViewProjectionTransform");
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
	}
}

unsigned int Renderer::GetTextureID(TextureType texture) {
	return sTextureID[texture];
}

//
// The following code is taken from
// www.opengl-tutorial.org
//
GLuint Renderer::LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint Renderer::LoadTexture(string texture_path_s) {
	char* texture_path = &texture_path_s[0];
	// Load image using the Free Image library
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(texture_path, 0);
	FIBITMAP* image = FreeImage_Load(format, texture_path);
	FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);

	// Get an available texture index from OpenGL
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	assert(textureID != 0);

	// Set OpenGL filtering properties (bi-linear interpolation)
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Retrieve width and hight
	int width = FreeImage_GetWidth(image32bits);
	int height = FreeImage_GetHeight(image32bits);

	// This will upload the texture to the GPU memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));

	// Free images
	FreeImage_Unload(image);
	FreeImage_Unload(image32bits);

	return textureID;
}

bool Renderer::ShaderNeedsTexture() {
	ShaderType shadersThatNeedTexture[2] = { SHADER_TEXTURED, SHADER_TEXTURED_UNCOLORED };
	ShaderType *found = std::find(std::begin(shadersThatNeedTexture), std::end(shadersThatNeedTexture), ShaderType(GetCurrentShader()));
	return found != std::end(shadersThatNeedTexture);
}

bool Renderer::PrintError()
{
	static bool checkForErrors = true;

	//
	if (!checkForErrors)
	{
		return false;
	}

	//
	const char* errorString = NULL;
	bool retVal = false;

	switch (glGetError())
	{
	case GL_NO_ERROR:
		retVal = true;
		break;

	case GL_INVALID_ENUM:
		errorString = "GL_INVALID_ENUM";
		break;

	case GL_INVALID_VALUE:
		errorString = "GL_INVALID_VALUE";
		break;

	case GL_INVALID_OPERATION:
		errorString = "GL_INVALID_OPERATION";
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;

	case GL_OUT_OF_MEMORY:
		errorString = "GL_OUT_OF_MEMORY";
		break;

	default:
		errorString = "UNKNOWN";
		break;
	}

	//
	if (!retVal)
	{
		printf("%s\n", errorString);
	}

	//
	return retVal;
}


void Renderer::CheckForErrors()
{
	while (PrintError() == false)
	{
	}
}

