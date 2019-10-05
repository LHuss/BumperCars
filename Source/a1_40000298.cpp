//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
// Used as baseline for Assignment 1 by Loïc Huss on 02/10/2019
//

#include <iostream>
#include <list>
#include <algorithm>

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
	}

	void Update(float dt) {
		carPos += carVel * dt;
		rotationVec += rotationVel * dt;
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
	}

private:
	GLuint carWorldMatrix;
	vec3 carPos;
	vec3 carVel;
	vec3 rotationVec;
	vec3 rotationVel;
	vec3 carScale;
	GLenum drawMode;

	void drawBody() {
		mat4 worldMatrix = scaleAndRotate(translate(mat4(1.0f), carPos));

		glUniformMatrix4fv(carWorldMatrix, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(drawMode, 14, 36);
	}

	mat4 scaleAndRotate(mat4 translated) {
		mat4 worldMatrix = translated * scale(mat4(1.0f), carScale);
		if (rotationVec != vec3(0.0f, 0.0f, 0.0f)) {
			worldMatrix = worldMatrix * rotate(mat4(1.0f), radians(5.0f), rotationVec);
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
	vec3 darkGray = rgbToFloatV3(169, 169, 169); // for the wheels


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
								 

		vec3(-1.0f, 0.0f,-0.5f),	green,	// cube - dark gray, for body		14-49
		vec3(-1.0f, 0.0f, 0.5f),	green,
		vec3(-1.0f, 1.0f, 0.5f),	green,

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


		vec3(-0.5f, 0.0f,-0.5f),	silver,			// cube - silver, for rest			50-63
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

    // Create Window and rendering context using GLFW, resolution is 800x600
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
	vec3 initCameraPosition(5.0f, 10.0f, 5.0f);
	vec3 initCameraLookAt(0.0f, 0.0f, 0.0f);
	vec3 cameraPosition = initCameraPosition;
	vec3 cameraLookAt = initCameraLookAt;
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    
    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 10 * cameraSpeed; // 20 shift speed cuz gotta go fast boiiii
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool cameraFirstPerson = true; // press 1 or 2 to toggle this variable, or f5 to toggle
	double lastCameraToggle = -300;
	float minWaitToggle = 0.250f;
	    
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
	vec3 carPosition = vec3(0.0f, 0.0f, 0.0f); //Default car position : 0,0,0
	vec3 carVelocity = vec3(0.0f, 0.0f, 0.0f); //Honestly this one's just for whenever I implement shooting cars
	vec3 carRotation = vec3(0.0f, 0.0f, 0.0f); //Default car rotation: 0,0,0
	vec3 carRotationalVelocity = vec3(0.0f, 0.0f, 0.0f); //Again, the plan's just to make cars spin when I shoot 'em
	vec3 carScale = vec3(1.0f, 1.0f, 1.0f); //This one's actually an assignment specification at least
	Car mainCar(carPosition, carVelocity, carRotation, carRotationalVelocity, carScale, shaderProgram);

	// TODO - Shoot cars on left click lol
	list<Car> carProjectilesList;


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
		mainCar.Draw();


        // Draw in view space for first person camera
		// TODO
		// ADD SPINNING CAR IN VIEW FOR SHITS N GIGGLES
        if (cameraFirstPerson){
		}
		else {
		}

        
        
        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
		// this does strictly nothing as of yet, since we have no way to differentiate first/third person for now
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // change to first person
        {
            cameraFirstPerson = true;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // change to third person
        {
            cameraFirstPerson = false;
        }

		if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) // toggle camera state
		{
			double curTime = glfwGetTime();
			if ((curTime - minWaitToggle) > lastCameraToggle) {
				cameraFirstPerson = !cameraFirstPerson;
				lastCameraToggle = curTime;
			}
		}



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
        cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;
        
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
        
        cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
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

        // Set the view matrix for first and third person cameras
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center
		mat4 viewMatrix(1.0f);

		if (cameraFirstPerson) {
			viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		}
		else {
			// Position of the camera is on the sphere looking at the point of interest (cameraPosition)
			float radius = 5.0f;
			vec3 position = cameraPosition - vec3(
					radius * cosf(phi) * cosf(theta),
					radius * sinf(phi),
					-radius * cosf(phi) * sinf(theta)
				);
			viewMatrix = lookAt(position, cameraPosition, cameraUp);
		}

		GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			float randX = -20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40)));
			float randZ = -20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (40)));
			vec3 newPos(randX, 0.0f, randZ);
			mainCar.rePosition(newPos);
		}

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			mainCar.upScale();
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			mainCar.downScale();
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			carRotation += vec3(0.0f, 2.0f, 0.0f);
			mainCar.setRotation(carRotation);
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			carRotation -= vec3(0.0f, 2.0f, 0.0f);
			mainCar.setRotation(carRotation);
		}


		// TODO - ADD PROJECTILE CARS
    }

    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}
