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
		LoadShaders(shaderPathPrefix + "SolidColor.vert",
			shaderPathPrefix + "SolidColor.frag")
	);
	sShaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "Textured.vert",
			shaderPathPrefix + "Textured.frag")
	);
	sShaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "Textured_Uncolored.vert",
			shaderPathPrefix + "Textured_Uncolored.frag")
	);
	sShaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "Lighting.vert",
			shaderPathPrefix + "Lighting.frag")
	);
	sShaderProgramID.push_back(
		LoadShaders(shaderPathPrefix + "Lighting_Textured.vert",
			shaderPathPrefix + "Lighting_Textured.frag")
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
	if (currentShader != type && type != ShaderType::SHADER_NULL) {
		SetShader(type);

		glUseProgram(GetShaderProgramID());

		BindUniforms();
	}
}

void Renderer::BindUniforms() {
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	glm::mat4 VP = World::GetInstancedViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Do the same shit, but for light data
	GLuint ViewPosLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "viewPos");
	glm::vec3 viewPos = World::GetInstance()->GetCurrentCamera()->GetPosition();
	glUniform3f(ViewPosLocation, viewPos.x, viewPos.y, viewPos.z);


	vector<PointLight*> pointLights = World::GetInstance()->GetPointLights();
	int numPointLights = 0;
	for (vector<PointLight*>::iterator it = pointLights.begin(); it < pointLights.end(); ++it) {
		PointLight* light = *it;
		if (light->IsActive()) {
			string lightLocS = string("pointLights[") + to_string(numPointLights) + "]";

			char* lightPosL = ToCharArray((lightLocS + ".position"));
			GLuint lightPosLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), lightPosL);
			glm::vec3 lightPos = light->GetPosition();
			glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

			char* lightColL = ToCharArray((lightLocS + ".color"));
			GLuint lightColorLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightColL);
			glm::vec3 lightCol = light->GetColor();
			glUniform3f(lightColorLoc, lightCol.x, lightCol.y, lightCol.z);

			char* lightAmbL = ToCharArray((lightLocS + ".ambient"));
			GLuint lightAmbientLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightAmbL);
			glm::vec3 lightAmb = light->GetAmbient();
			char* lightDiffL = ToCharArray((lightLocS + ".diffuse"));
			glUniform3f(lightAmbientLoc, lightAmb.x, lightAmb.y, lightAmb.z);
			GLuint lightDiffuseLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightDiffL);
			glm::vec3 lightDiff = light->GetDiffuse();
			char* lightSpecL = ToCharArray((lightLocS + ".specular"));
			glUniform3f(lightDiffuseLoc, lightDiff.x, lightDiff.y, lightDiff.z);
			GLuint lightSpecularLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightSpecL);
			glm::vec3 lightSpec = light->GetSpecular();
			glUniform3f(lightSpecularLoc, lightSpec.x, lightSpec.y, lightSpec.z);

			char* lightConstL = ToCharArray((lightLocS + ".constant"));
			GLuint lightConstantLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightConstL);
			float lightConst = light->GetConstant();
			glUniform1f(lightConstantLoc, lightConst);
			char* lightLinearL = ToCharArray((lightLocS + ".linear"));
			GLuint lightLinearLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightLinearL);
			float lightLinear = light->GetLinear();
			glUniform1f(lightLinearLoc, lightLinear);
			char* lightQuadL = ToCharArray((lightLocS + ".quadratic"));
			GLuint lightQuadraticLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightQuadL);
			float lightQuad = light->GetQuadratic();
			glUniform1f(lightQuadraticLoc, lightQuad);
			++numPointLights;
		}
	}
	GLuint numPointLightsLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "numPointLights");
	glUniform1i(numPointLightsLoc, numPointLights);

	vector<SpotLight*> spotLights = World::GetInstance()->GetSpotLights();
	int numSpotLights = 0;
	for (vector<SpotLight*>::iterator it = spotLights.begin(); it < spotLights.end(); ++it) {
		SpotLight* light = *it;
		if (light->IsActive()) {
			string lightLocS = string("spotLights[") + to_string(numSpotLights) + "]";

			char* lightPosL = ToCharArray((lightLocS + ".position"));
			GLuint lightPosLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), lightPosL);
			glm::vec3 lightPos = light->GetPosition();
			glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

			char* lightColL = ToCharArray((lightLocS + ".color"));
			GLuint lightColorLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightColL);
			glm::vec3 lightCol = light->GetColor();
			glUniform3f(lightColorLoc, lightCol.x, lightCol.y, lightCol.z);

			char* lightDirL = ToCharArray((lightLocS + ".direction"));
			GLuint lightDirLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightDirL);
			glm::vec3 lightDir = light->GetDirection();
			glUniform3f(lightDirLoc, lightDir.x, lightDir.y, lightDir.z);

			char* lightCOL = ToCharArray((lightLocS + ".cutOff"));
			GLuint lightCOLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightCOL);
			float lightCO = light->GetCutOff();
			glUniform1f(lightCOLoc, lightCO);
			char* lightOCOL = ToCharArray((lightLocS + ".outerCutOff"));
			GLuint lightOCOLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightOCOL);
			float lightOCO = light->GetCutOff();
			glUniform1f(lightOCOLoc, lightOCO);


			char* lightAmbL = ToCharArray((lightLocS + ".ambient"));
			GLuint lightAmbientLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightAmbL);
			glm::vec3 lightAmb = light->GetAmbient();
			char* lightDiffL = ToCharArray((lightLocS + ".diffuse"));
			glUniform3f(lightAmbientLoc, lightAmb.x, lightAmb.y, lightAmb.z);
			GLuint lightDiffuseLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightDiffL);
			glm::vec3 lightDiff = light->GetDiffuse();
			char* lightSpecL = ToCharArray((lightLocS + ".specular"));
			glUniform3f(lightDiffuseLoc, lightDiff.x, lightDiff.y, lightDiff.z);
			GLuint lightSpecularLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightSpecL);
			glm::vec3 lightSpec = light->GetSpecular();
			glUniform3f(lightSpecularLoc, lightSpec.x, lightSpec.y, lightSpec.z);

			char* lightConstL = ToCharArray((lightLocS + ".constant"));
			GLuint lightConstantLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightConstL);
			float lightConst = light->GetConstant();
			glUniform1f(lightConstantLoc, lightConst);
			char* lightLinearL = ToCharArray((lightLocS + ".linear"));
			GLuint lightLinearLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightLinearL);
			float lightLinear = light->GetLinear();
			glUniform1f(lightLinearLoc, lightLinear);
			char* lightQuadL = ToCharArray((lightLocS + ".quadratic"));
			GLuint lightQuadraticLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), lightQuadL);
			float lightQuad = light->GetQuadratic();
			glUniform1f(lightQuadraticLoc, lightQuad);
			++numSpotLights;
		}
	}
	GLuint numSpotLightsLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "numSpotLights");
	glUniform1i(numSpotLightsLoc, numSpotLights);

	GLuint numDirLightsLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "numDirLights");
	glUniform1i(numDirLightsLoc, 0);
}

char* Renderer::ToCharArray(string in) {
	char* out = new char[in.size() + 1];
	strcpy(out, in.c_str());
	return out;
}

void Renderer::BindTextureUniforms(TextureType texture) {
	GLuint MatAmbientLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.ambient");
	glUniform1f(MatAmbientLoc, GetAmbientStrength(texture));
	GLuint MatSpecularLoc = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.specular");
	glUniform1f(MatSpecularLoc, GetSpecularStrength(texture));
	GLuint MatShininess = glGetUniformLocation(Renderer::GetShaderProgramID(), "material.shininess");
	glUniform1f(MatShininess, GetShininess(texture));
}

unsigned int Renderer::GetTextureID(TextureType texture) {
	if (texture == TextureType::TEXTURE_NULL) {
		return 0;
	}
	else {
		return sTextureID[texture];
	}
}

float Renderer::GetShininess(TextureType texture) {
	map<unsigned int, float> shininessMap;

	shininessMap[TextureType::TEXTURE_BRICK] = 10.0f;
	shininessMap[TextureType::TEXTURE_CEMENT] = 20.0f;
	shininessMap[TextureType::TEXTURE_GRASS] = 15.0f;
	shininessMap[TextureType::TEXTURE_STEEL] = 40.0f;
	shininessMap[TextureType::TEXTURE_TIRE] = 10.0f;
	shininessMap[TextureType::TEXTURE_WOOD] = 10.0f;
	shininessMap[TextureType::TEXTURE_NULL] = 32.0f;

	return shininessMap[texture];
}

float Renderer::GetSpecularStrength(TextureType texture) {
	map<unsigned int, float> specularStrengthMap;

	specularStrengthMap[TextureType::TEXTURE_BRICK] = 0.1f;
	specularStrengthMap[TextureType::TEXTURE_CEMENT] = 0.1f;
	specularStrengthMap[TextureType::TEXTURE_GRASS] = 0.3f;
	specularStrengthMap[TextureType::TEXTURE_STEEL] = 0.8f;
	specularStrengthMap[TextureType::TEXTURE_TIRE] = 0.1f;
	specularStrengthMap[TextureType::TEXTURE_WOOD] = 0.4f;
	specularStrengthMap[TextureType::TEXTURE_NULL] = 0.5f;

	return specularStrengthMap[texture];
}

float Renderer::GetAmbientStrength(TextureType texture) {
	map<unsigned int, float> ambientStrength;

	ambientStrength[TextureType::TEXTURE_BRICK] = 0.1f;
	ambientStrength[TextureType::TEXTURE_CEMENT] = 0.1f;
	ambientStrength[TextureType::TEXTURE_GRASS] = 0.1f;
	ambientStrength[TextureType::TEXTURE_STEEL] = 0.6f;
	ambientStrength[TextureType::TEXTURE_TIRE] = 0.1f;
	ambientStrength[TextureType::TEXTURE_WOOD] = 0.4f;
	ambientStrength[TextureType::TEXTURE_NULL] = 0.5f;

	return ambientStrength[texture];
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
	ShaderType shadersThatNeedTexture[3] = { ShaderType::SHADER_TEXTURED, ShaderType::SHADER_TEXTURED_UNCOLORED, ShaderType::SHADER_LIGHTING_TEXTURED };
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
