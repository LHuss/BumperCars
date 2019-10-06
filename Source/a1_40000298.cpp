//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
// Used as baseline for Assignment 1 by Lo�c Huss on 02/10/2019
//

#include <iostream>
#include <list>
#include <algorithm>
#include <math.h> // fmod

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

using namespace glm;
using namespace std;

class Car
{
public:
	Car(vec3 position, vec3 velocity, vec3 rotationalVector, vec3 rotationalVelocity, vec3 scale, int shaderProgram) : carPos(position), carVel(velocity), rotationVec(rotationalVector), rotationVel(rotationalVelocity), carScale(scale)
	{
		carWorldMatrix = glGetUniformLocation(shaderProgram, "worldMatrix");
		drawMode = GL_TRIANGLES;
		angleStepSize = 5.0f;
		wheelAngle = 0.0f;
	}

	void UpdateWheels(float dt) {
		wheelAngle += 180.0f * dt;
	}

	// keep update in case I want to make projectile cars
	void Update(float dt) {
		carPos += carVel * dt;
		rotationVec += rotationVel * dt;
		UpdateWheels(dt);
	}

	void setRotation(vec3 rotationalVector) {
		rotationVec = rotationalVector;
	}

	void setDrawMode(GLenum mode) {
		drawMode = mode;
	}

	void rePosition(vec3 position) {
		carPos = position;
	}

	void upScale() {
		carScale *= 1.05;
	}

	void downScale() {
		carScale /= 1.05;
	}

	void Draw() {
		drawBody();
		drawRoof();
		drawBonnet();
		drawTrunk();
		drawWheels();
	}

private:
	GLuint carWorldMatrix;
	vec3 carPos;
	vec3 carVel;
	vec3 rotationVec;
	vec3 rotationVel;
	vec3 carScale;
	GLenum drawMode;
	float angleStepSize;
	float wheelAngle;

	void drawBody() {
		mat4 worldMatrix = scaleAndRotate(translate(mat4(1.0f), carPos));

		glUniformMatrix4fv(carWorldMatrix, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(drawMode, 14, 36);
	}

	void drawRoof() {
		vec3 roofShift = vec3(0.0f, 1.0f, 0.0f) * carScale;
		vec3 roofPos = carPos + shiftDueToRotation(roofShift);
		mat4 roofMatrix = translate(mat4(1.0f), roofPos) * scale(mat4(1.0f), vec3(1.0f, 0.5f, 0.5f));
		mat4 worldMatrix = scaleAndRotate(roofMatrix);

		glUniformMatrix4fv(carWorldMatrix, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(drawMode, 50, 36);
	}

	void drawBonnet() {
		vec3 bonnetShift = vec3(1.3625f, 0.0f, 0.0f) * carScale;
		vec3 bonnetPos = carPos + shiftDueToRotation(bonnetShift);
		mat4 bonnetMatrix = translate(mat4(1.0f), bonnetPos) * scale(mat4(1.0f), vec3(0.75f, 0.6f, 1.0f));
		mat4 worldMatrix = scaleAndRotate(bonnetMatrix);

		glUniformMatrix4fv(carWorldMatrix, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(drawMode, 50, 36);
	}

	void drawTrunk() {
		vec3 trunkShift = vec3(-1.25f, 0.0f, 0.0f) * carScale;
		vec3 trunkPos = carPos + shiftDueToRotation(trunkShift);
		mat4 trunkMatrix = translate(mat4(1.0f), trunkPos) * scale(mat4(1.0f), vec3(0.5f, 0.8f, 1.0f));
		mat4 worldMatrix = scaleAndRotate(trunkMatrix);

		glUniformMatrix4fv(carWorldMatrix, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(drawMode, 50, 36);
	}

	void drawWheels() {
		float xPos[] = { -0.6f, 0.6f };
		float zPos[] = { -0.5f, 0.5f };
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				vec3 wheelShift = vec3(xPos[i], 0.0f, zPos[j]) * carScale;
				vec3 wheelPos = carPos + shiftDueToRotation(wheelShift);
				mat4 wheelMatrix = translate(mat4(1.0f), wheelPos) * rotate(mat4(1.0f), radians(wheelAngle), vec3(0.0f, 0.0f, -1.0f));
				mat4 worldMatrix = scaleAndRotate(wheelMatrix);

				glUniformMatrix4fv(carWorldMatrix, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawArrays(drawMode, 86, 36);
			}
		}
	}

	// This is all kinds of wrong, but my brain doesn't want to understand how math works
	// So I guess it's a feature for now...
	// I'm hours in at this point and I can't... Wtf is you doing, brain?
	// Whatever, this is GG I guess...
	vec3 shiftDueToRotation(vec3 unshifted) {
		float xAngle = radians(rotationVec.x * angleStepSize); // affects tilt (yz)
		float zAngle = radians(rotationVec.z * angleStepSize); // affects updown (xy)
		float yAngle = radians(rotationVec.y * angleStepSize); // affects sideways (xz)
		vec3 shifted(0.0f, 0.0f, 0.0f);

		// Probably needed to use radius somehow, but honestly nothing's working anyways so I give in
		float radius = sqrtf(powf(unshifted.x, 2) + powf(unshifted.y, 2) + powf(unshifted.z, 2));

		// Shifts due to x angle:
		shifted.x += unshifted.x * cosf(xAngle);
		shifted.z += unshifted.x * sinf(xAngle);

		// Shifts due to y angle:
		shifted.y += unshifted.y * cosf(yAngle);
		shifted.z += unshifted.y * sinf(yAngle);

		// Shifts due to z angle:
		shifted.z += unshifted.z * cosf(zAngle);
		shifted.x += unshifted.z * sinf(zAngle);

		return shifted;
	}

	mat4 scaleAndRotate(mat4 baseMatrix) {
		mat4 worldMatrix = baseMatrix * scale(mat4(1.0f), carScale);
		if (rotationVec != vec3(0.0f, 0.0f, 0.0f)) {
			worldMatrix = worldMatrix 
				* rotate(mat4(1.0f), radians(angleStepSize * rotationVec.x), vec3(1.0f, 0.0f, 0.0f))
				* rotate(mat4(1.0f), radians(angleStepSize * rotationVec.y), vec3(0.0f, 1.0f, 0.0f))
				* rotate(mat4(1.0f), radians(angleStepSize * rotationVec.z), vec3(0.0f, 0.0f, 1.0f));
		}

		return worldMatrix;
	}
};

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
        "}";
}

int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

vec3 rgbToFloatV3(int r, int g, int b) {
	return vec3((float)r / 255.0, (float)g / 255.0, (float)b / 255.0);
}

int createVertexBufferObject()
{
	vec3 red = rgbToFloatV3(255, 0, 0);
	vec3 green = rgbToFloatV3(0, 255, 0);
	vec3 blue = rgbToFloatV3(0, 0, 255);

	vec3 lightBlue = rgbToFloatV3(68, 85, 90);
	vec3 darkSlateGray = rgbToFloatV3(47, 79, 79); // for the body
	vec3 silver = rgbToFloatV3(192, 192, 192); // for bonnet/trunk/roof
	vec3 gold = rgbToFloatV3(255, 215, 0); // for the wheels


	vec3 vertexArray[] = {
		// position					//color
		vec3(0.0f, 0.0f, 0.0f),		lightBlue,		// Light blue square - for grid		0-7
		vec3(0.0f, 0.0f, 1.0f),		lightBlue,

		vec3(0.0f, 0.0f, 1.0f),		lightBlue,
		vec3(1.0f, 0.0f, 1.0f),		lightBlue,

		vec3(1.0f, 0.0f, 1.0f),		lightBlue,
		vec3(1.0f, 0.0f, 0.0f),		lightBlue,

		vec3(1.0f, 0.0f, 0.0f),		lightBlue,
		vec3(0.0f, 0.0f, 0.0f),		lightBlue,


		vec3(0.0f, 0.0f, 0.0f),		red,			// Line along x, red				8-9
		vec3(1.0f, 0.0f, 0.0f),		red,

		vec3(0.0f, 0.0f, 0.0f),		green,			// Line along y, green				10-11
		vec3(0.0f, 1.0f, 0.0f),		green,

		vec3(0.0f, 0.0f, 0.0f),		blue,			// Line along z, blue				12-13
		vec3(0.0f, 0.0f, 1.0f),		blue,
								 

		vec3(-1.0f, 0.0f,-0.5f),	darkSlateGray,	// box - dark gray, for body		14-49
		vec3(-1.0f, 0.0f, 0.5f),	darkSlateGray,
		vec3(-1.0f, 1.0f, 0.5f),	darkSlateGray,

		vec3(-1.0f, 0.0f,-0.5f),	darkSlateGray,
		vec3(-1.0f, 1.0f, 0.5f),	darkSlateGray,
		vec3(-1.0f, 1.0f,-0.5f),	darkSlateGray,

		vec3(1.0f, 1.0f,-0.5f),		darkSlateGray,
		vec3(-1.0f, 0.0f,-0.5f),	darkSlateGray,
		vec3(-1.0f, 1.0f,-0.5f),	darkSlateGray,

		vec3(1.0f, 1.0f,-0.5f),		darkSlateGray,
		vec3(1.0f, 0.0f,-0.5f), 	darkSlateGray,
		vec3(-1.0f, 0.0f,-0.5f), 	darkSlateGray,

		vec3(1.0f, 0.0f, 0.5f), 	darkSlateGray,
		vec3(-1.0f, 0.0f,-0.5f), 	darkSlateGray,
		vec3(1.0f, 0.0f,-0.5f), 	darkSlateGray,

		vec3(1.0f, 0.0f, 0.5f), 	darkSlateGray,
		vec3(-1.0f, 0.0f, 0.5f), 	darkSlateGray,
		vec3(-1.0f, 0.0f,-0.5f), 	darkSlateGray,

		vec3(-1.0f, 1.0f, 0.5f), 	darkSlateGray,
		vec3(-1.0f, 0.0f, 0.5f), 	darkSlateGray,
		vec3(1.0f, 0.0f, 0.5f), 	darkSlateGray,

		vec3(1.0f, 1.0f, 0.5f), 	darkSlateGray,
		vec3(-1.0f, 1.0f, 0.5f), 	darkSlateGray,
		vec3(1.0f, 0.0f, 0.5f), 	darkSlateGray,

		vec3(1.0f, 1.0f, 0.5f), 	darkSlateGray,
		vec3(1.0f, 0.0f,-0.5f), 	darkSlateGray,
		vec3(1.0f, 1.0f,-0.5f), 	darkSlateGray,

		vec3(1.0f, 0.0f,-0.5f), 	darkSlateGray,
		vec3(1.0f, 1.0f, 0.5f), 	darkSlateGray,
		vec3(1.0f, 0.0f, 0.5f), 	darkSlateGray,

		vec3(1.0f, 1.0f, 0.5f), 	darkSlateGray,
		vec3(1.0f, 1.0f,-0.5f), 	darkSlateGray,
		vec3(-1.0f, 1.0f,-0.5f), 	darkSlateGray,

		vec3(1.0f, 1.0f, 0.5f), 	darkSlateGray,
		vec3(-1.0f, 1.0f,-0.5f), 	darkSlateGray,
		vec3(-1.0f, 1.0f, 0.5f), 	darkSlateGray,


		vec3(-0.5f, 0.0f,-0.5f),	silver,			// box - silver, for rest			50-85
		vec3(-0.5f, 0.0f, 0.5f),	silver,
		vec3(-0.5f, 1.0f, 0.5f),	silver,

		vec3(-0.5f, 0.0f,-0.5f),	silver,
		vec3(-0.5f, 1.0f, 0.5f),	silver,
		vec3(-0.5f, 1.0f,-0.5f),	silver,

		vec3(0.5f, 1.0f,-0.5f),		silver,
		vec3(-0.5f, 0.0f,-0.5f),	silver,
		vec3(-0.5f, 1.0f,-0.5f),	silver,

		vec3(0.5f, 1.0f,-0.5f),		silver,
		vec3(0.5f, 0.0f,-0.5f), 	silver,
		vec3(-0.5f, 0.0f,-0.5f), 	silver,

		vec3(0.5f, 0.0f, 0.5f), 	silver,
		vec3(-0.5f, 0.0f,-0.5f), 	silver,
		vec3(0.5f, 0.0f,-0.5f), 	silver,

		vec3(0.5f, 0.0f, 0.5f), 	silver,
		vec3(-0.5f, 0.0f, 0.5f), 	silver,
		vec3(-0.5f, 0.0f,-0.5f), 	silver,

		vec3(-0.5f, 1.0f, 0.5f), 	silver,
		vec3(-0.5f, 0.0f, 0.5f), 	silver,
		vec3(0.5f, 0.0f, 0.5f), 	silver,

		vec3(0.5f, 1.0f, 0.5f),		silver,
		vec3(-0.5f, 1.0f, 0.5f),	silver,
		vec3(0.5f, 0.0f, 0.5f),		silver,

		vec3(0.5f, 1.0f, 0.5f),		silver,
		vec3(0.5f, 0.0f,-0.5f),		silver,
		vec3(0.5f, 1.0f,-0.5f),		silver,

		vec3(0.5f, 0.0f,-0.5f),		silver,
		vec3(0.5f, 1.0f, 0.5f),		silver,
		vec3(0.5f, 0.0f, 0.5f), 	silver,

		vec3(0.5f, 1.0f, 0.5f),		silver,
		vec3(0.5f, 1.0f,-0.5f),		silver,
		vec3(-0.5f, 1.0f,-0.5f),	silver,

		vec3(0.5f, 1.0f, 0.5f),		silver,
		vec3(-0.5f, 1.0f,-0.5f),	silver,
		vec3(-0.5f, 1.0f, 0.5f),	silver,

		// probably another one for wheels, idk yet?
		vec3(-0.25f, -0.25f, -0.25f),	gold,			// box - gold, for wheels		86-121
		vec3(-0.25f, -0.25f, 0.25f),	gold,
		vec3(-0.25f, 0.25f, 0.25f),	gold,

		vec3(-0.25f, -0.25f, -0.25f),	gold,
		vec3(-0.25f, 0.25f, 0.25f),	gold,
		vec3(-0.25f, 0.25f, -0.25f),	gold,

		vec3(0.25f, 0.25f, -0.25f),	gold,
		vec3(-0.25f, -0.25f, -0.25f),	gold,
		vec3(-0.25f, 0.25f, -0.25f),	gold,

		vec3(0.25f, 0.25f, -0.25f),	gold,
		vec3(0.25f, -0.25f, -0.25f),	gold,
		vec3(-0.25f, -0.25f, -0.25f),	gold,

		vec3(0.25f, -0.25f, 0.25f),	gold,
		vec3(-0.25f, -0.25f, -0.25f),	gold,
		vec3(0.25f, -0.25f, -0.25f),	gold,

		vec3(0.25f, -0.25f, 0.25f),	gold,
		vec3(-0.25f, -0.25f, 0.25f),	gold,
		vec3(-0.25f, -0.25f, -0.25f),	gold,

		vec3(-0.25f, 0.25f, 0.25f),	gold,
		vec3(-0.25f, -0.25f, 0.25f),	gold,
		vec3(0.25f, -0.25f, 0.25f),	gold,

		vec3(0.25f, 0.25f, 0.25f),	gold,
		vec3(-0.25f, 0.25f, 0.25f),	gold,
		vec3(0.25f, -0.25f, 0.25f),	gold,

		vec3(0.25f, 0.25f, 0.25f),	gold,
		vec3(0.25f, -0.25f, -0.25f),	gold,
		vec3(0.25f, 0.25f, -0.25f),	gold,

		vec3(0.25f, -0.25f, -0.25f),	gold,
		vec3(0.25f, 0.25f, 0.25f),	gold,
		vec3(0.25f, -0.25f, 0.25f),	gold,

		vec3(0.25f, 0.25f, 0.25f),	gold,
		vec3(0.25f, 0.25f, -0.25f),	gold,
		vec3(-0.25f, 0.25f, -0.25f),	gold,

		vec3(0.25f, 0.25f, 0.25f),	gold,
		vec3(-0.25f, 0.25f, -0.25f),	gold,
		vec3(-0.25f, -0.25f, 0.25f),	gold,
	};
	    
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(vec3),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);
    
    return vertexBufferObject;
}

int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();
    
#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 1024x768
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Lab 03", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Disable Mouse Cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();
    
    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    // Camera parameters for view transform
	vec3 initCameraPosition(5.0f, 30.0f, 50.0f);
	vec3 initCameraLookAt(0.0f, -5.0f, 0.0f);
	vec3 cameraPosition = initCameraPosition;
	vec3 cameraLookAt = initCameraLookAt;
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    
    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 10 * cameraSpeed; // 10 shift speed cuz gotta go fast boiiii
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;

	// Minimum time to wait between certain keybinds
	float minKeyWait = 0.250f;

	// Last time certain keys are pressed
	double lastCameraToggle = -300;
	double lastSpace = -300;
	double lastE = -300;
	double lastQ = -300;
	double lastR = -300;
	double lastG = -300;
	double lastF = -300;
	double lastH = -300;
	    
    // Set projection matrix for shader
    mat4 projectionMatrix = glm::perspective(90.0f,            // field of view in degrees (90 to feel fast af)
                                             1024.0f / 768.0f,  // aspect ratio
                                             0.01f, 100.0f);   // near and far (near > 0)
    
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,				 // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp );					 // up
    
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    
    
    // Define and upload geometry to the GPU here ...
    int vbo = createVertexBufferObject();
    
    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    
    // Enable hidden surface removal
    glEnable(GL_CULL_FACE);

	// Main car object
	vec3 defaultCarPos(0.0f, 0.25f, 0.0f);	//Default car position : 0,0.25,0
	vec3 carPosition = defaultCarPos; 
	vec3 defaultCarVel(0.0f, 0.0f, 0.0f);	//Honestly this one's just for whenever I implement shooting cars
	vec3 carVelocity = defaultCarVel;
	vec3 defaultCarRot(0.0f, 0.0f, 0.0f);	//Default car rotation: 0,0,0
	vec3 carRotation = defaultCarRot;
	vec3 defaulRotVel(0.0f, 0.0f, 0.0f);	//Again, the plan's just to make cars spin when I shoot 'em
	vec3 carRotationalVelocity = defaulRotVel;
	vec3 defaultCarScale(1.0f, 1.0f, 1.0f);
	vec3 carScale = defaultCarScale;		//This one's actually an assignment specification at least
	Car mainCar(carPosition, carVelocity, carRotation, carRotationalVelocity, carScale, shaderProgram);
	
	// other car params
	float carSpeed = 1.0f;

	// Mouse params
	bool isHoldingRight = false;
	bool isHoldingLeft = false;
	bool isHoldingMiddle = false;
	float cameraPanSpeed = 15.0f;
	float cameraTiltSpeed = 20.0f;

	// Shoot cars on left click lol
	list<Car> carProjectilesList;

	// custom "pointer"
	float spinningCarAngle = 0.0f;
	vec3 spinningRotVel = vec3(1.0f, 1.0f, 1.0f);
	vec3 spinningCarScale = vec3(0.01f, 0.01f, 0.01f);
	Car pointerCar(defaultCarPos, defaultCarVel, defaultCarRot, spinningRotVel, spinningCarScale, shaderProgram);

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

		glEnable(GL_DEPTH_TEST); 
		// Add the GL_DEPTH_BUFFER_BIT to glClear - Basically just draw close objects on top
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw geometry
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

#pragma region Draw

		// World Matrix
		GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

		// Draw Axial Vertices
		mat4 axeWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &axeWorldMatrix[0][0]);
		glDrawArrays(GL_LINES, 8, 6); // 6 vertices (2 per axial vertex), starting at index 8


		// Draw grid
		mat4 gridWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
		glDrawArrays(GL_LINES, 0, 8); // 8 vertices (2 per line), starting at index 0

		for (int i = -50; i < 50; i++) {
			for (int j = -50; j < 50; j++) {
				if (i == 0 && j == 0) { continue; } // "center" tile already drawn above

				gridWorldMatrix = translate(mat4(1.0f), vec3(0.0f + i, 0.0f, 0.0f + j));
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorldMatrix[0][0]);
				glDrawArrays(GL_LINES, 0, 8);
			}
		}

		// Draw car
		mainCar.UpdateWheels(dt);
		mainCar.Draw();

		// Update and draw projectiles
		for (list<Car>::iterator it = carProjectilesList.begin(); it != carProjectilesList.end(); ++it) {
			it->Update(dt);
			it->Draw();
		}

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

#pragma endregion
	
#pragma region Inputs
		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		double curTime = glfwGetTime();

#pragma region Camera Inputs
#pragma region Not Car Camera
		bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
		float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

		// Calculate mouse motion dx and dy
		// Update camera horizontal and vertical angle
		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Convert to spherical coordinates
		const float cameraAngularSpeed = 60.0f;
		cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
		cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

		vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

		glm::normalize(cameraSideVector);


		// use camera lookat and side vectors to update positions with Up/Down/Left/Right
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera down
		{
			cameraPosition += cameraLookAt * currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera up
		{
			cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move camera to the left
		{
			cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move camera to the right
		{
			cameraPosition += cameraSideVector * currentCameraSpeed * dt;
		}

#pragma endregion

#pragma region Car Mouse Controls
		bool freezeCamera = false;


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (isHoldingLeft) {
				cameraPosition -= cameraLookAt * (float) dy;
			}
			isHoldingLeft = true;
		}
		else {
			isHoldingLeft = false;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			if (isHoldingRight) {
				cameraPosition -= cameraSideVector * (float) dx * cameraPanSpeed * dt;
			}
			isHoldingRight = true;
		}
		else {
			isHoldingRight = false;
		}


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
			if (isHoldingMiddle) {
				cameraPosition += vec3(0.0f, dy, 0.0f) * cameraTiltSpeed * dt;
			}
			isHoldingMiddle = true;
		}
		else {
			isHoldingMiddle = false;
		}
#pragma endregion

		// Set the view matrix.
		mat4 viewMatrix(1.0f);
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

#pragma endregion

#pragma region Main Car Controls
#pragma region Car Key Controls
		// W, A, S, D - Move car fowards/left/back/rightwAdwasd
		float carMovement = carSpeed * dt;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			carPosition += vec3(carMovement, 0.0f, 0.0f);
			mainCar.rePosition(carPosition);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			carPosition += vec3(-carMovement, 0.0f, 0.0f);
			mainCar.rePosition(carPosition);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			carPosition += vec3(0.0f, 0.0f, -carMovement);
			mainCar.rePosition(carPosition);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			carPosition += vec3(0.0f, 0.0f, carMovement);
			mainCar.rePosition(carPosition);
		}

		// P, L, T - Change draw modes for the car
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			mainCar.setDrawMode(GL_POINTS);
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			mainCar.setDrawMode(GL_LINES);
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			mainCar.setDrawMode(GL_TRIANGLES);
		}

		// HOME - Reset Car
		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
			carPosition = defaultCarPos;
			mainCar = Car(defaultCarPos, defaultCarVel, defaultCarRot, defaulRotVel, defaultCarScale, shaderProgram);
			cameraPosition = initCameraPosition;
			cameraLookAt = initCameraLookAt;
		}

		// SPACE - Draw car  randomly in a 40 block radius around the center
		// Could be 50, but I didn't like it when it spawned on the edge
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			if ((curTime - minKeyWait) > lastSpace) {
				float randX = -40 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (80)));
				float randZ = -40 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (80)));
				carPosition = vec3(randX, 0.25f, randZ);
				mainCar.rePosition(carPosition);
				lastSpace = curTime;
			}
		}

		// U, J - Scale Up/Down car respectively
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			mainCar.upScale();
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			mainCar.downScale();
		}

		// Q, E - Rotate car along x-axis
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			if ((curTime - minKeyWait) > lastQ) {
				carRotation += vec3(0.0f, 1.0f, 0.0f);
				mainCar.setRotation(carRotation);
				lastQ = curTime;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			if ((curTime - minKeyWait) > lastE) {
				carRotation -= vec3(0.0f, 1.0f, 0.0f);
				mainCar.setRotation(carRotation);
				lastE = curTime;
			}
		}

		// R, G - Rotate car along z-axis
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			if ((curTime - minKeyWait) > lastR) {
				carRotation += vec3(1.0f, 0.0f, 0.0f);
				mainCar.setRotation(carRotation);
				lastR = curTime;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			if ((curTime - minKeyWait) > lastG) {
				carRotation -= vec3(1.0f, 0.0f, 0.0f);
				mainCar.setRotation(carRotation);
				lastG = curTime;
			}
		}

		// F, H - Rotate car along y-axis
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			if ((curTime - minKeyWait) > lastF) {
				carRotation += vec3(0.0f, 0.0f, 1.0f);
				mainCar.setRotation(carRotation);
				lastF = curTime;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			if ((curTime - minKeyWait) > lastH) {
				carRotation -= vec3(0.0f, 0.0f, 1.0f);
				mainCar.setRotation(carRotation);
				lastH = curTime;
			}
		}
#pragma endregion
#pragma endregion

		// ADD PROJECTILE CARS
		if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			const float projectileSpeed = 5.0f;
			
			Car projectile(cameraPosition, cameraLookAt*projectileSpeed, vec3(0.0f, 0.0f, 0.0f), vec3(10.0f, 0.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f), shaderProgram);
			if (carProjectilesList.size() > 10) {
				carProjectilesList.pop_front();
			}
			carProjectilesList.push_back(projectile);
		}
		lastMouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
#pragma endregion
    }

    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}
