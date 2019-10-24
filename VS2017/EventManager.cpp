#include "EventManager.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;


// Time
double EventManager::sLastFrameTime = glfwGetTime();
float  EventManager::sFrameTime = 0.0f;

// Key Delays
const float EventManager::sMinKeyWait = 0.250f;
float EventManager::sLastSpace = -0.250f;

// Resolutions
std::vector<WindowResolution> EventManager::sPossibleResolutions;
WindowResolution EventManager::sWindowResolution;
int EventManager::sCurrentResolution = 0;

// Mouse
double EventManager::sLastMousePositionX = 0.0f;
float  EventManager::sMouseDeltaX = 0.0f;
double EventManager::sLastMousePositionY = 0.0f;
float  EventManager::sMouseDeltaY = 0.0f;

// Window
GLFWwindow* EventManager::spWindow = nullptr;


void EventManager::Initialize()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

#if defined(PLATFORM_OSX)
	// OS X would use legacy opengl by default, and wouldn't be able to load shaders
	// This is the proper way to setup GLFW to use modern OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);

#else
	// Allow older laptops to run the framework, even though, our framework
	// is compatible with OpenGL 3.3 (which removed many deprecated features)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
#endif


	// Open a window and create its OpenGL context
	glfwWindowHint(GLFW_RESIZABLE, 0);

	sPossibleResolutions.push_back({ 1024, 768 });
	sPossibleResolutions.push_back({ 1280, 1024 });
	sPossibleResolutions.push_back({ 1920, 1080 });
	sCurrentResolution = 0;
	sWindowResolution = sPossibleResolutions[sCurrentResolution];
	spWindow = glfwCreateWindow(sWindowResolution.width, sWindowResolution.height, "COMP371 - Assignment 2 - 40000298", nullptr, nullptr);

	if (spWindow == nullptr)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		exit(-1);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(spWindow, GLFW_STICKY_KEYS, GL_TRUE);

	// Initial mouse position
	glfwPollEvents();
	double x, y;
	glfwGetCursorPos(spWindow, &x, &y);

	sLastMousePositionX = x;
	sLastMousePositionY = y;

	// Initial time
	sLastFrameTime = glfwGetTime();
	srand((unsigned int)time(nullptr));
}

void EventManager::Shutdown()
{
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	spWindow = nullptr;
}

void EventManager::Update()
{
	// Update inputs / events
	glfwPollEvents();

	// Update mouse position
	double x, y;
	glfwGetCursorPos(spWindow, &x, &y);
	sMouseDeltaX = static_cast<float>(x - sLastMousePositionX);
	sMouseDeltaY = static_cast<float>(y - sLastMousePositionY);
	sLastMousePositionX = x;
	sLastMousePositionY = y;

	// Update frame time
	double currentTime = glfwGetTime();
	sFrameTime = static_cast<float>(currentTime - sLastFrameTime);
	sLastFrameTime = currentTime;

	// Change resolution
	if (glfwGetKey(spWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
		ToggleResolution();
	}
}

void EventManager::ToggleResolution() {
	sCurrentResolution = (sCurrentResolution + 1) % 3;
	ReloadWindow();
}

void EventManager::ReloadWindow() {
	sWindowResolution = sPossibleResolutions[sCurrentResolution];
	glViewport(0, 0, sWindowResolution.width, sWindowResolution.height);
	glfwSetWindowSize(spWindow, sWindowResolution.width, sWindowResolution.height);
}

float EventManager::GetFrameTime()
{
	return sFrameTime;
}

float EventManager::GetMinKeyWait() {
	return sMinKeyWait;
}

float EventManager::GetLastSpace() {
	return sLastSpace;
}

bool EventManager::ExitRequested()
{
	return glfwGetKey(spWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(spWindow);
}

GLFWwindow* EventManager::GetWindow()
{
	return spWindow;
}

float EventManager::GetMouseMotionX()
{
	return sMouseDeltaX;
}

float EventManager::GetMouseMotionY()
{
	return sMouseDeltaY;
}

WindowResolution EventManager::GetWindowResolution() {
	return sWindowResolution;
}

void EventManager::EnableMouseCursor()
{
	glfwSetInputMode(spWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void EventManager::DisableMouseCursor()
{
	glfwSetInputMode(spWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

float EventManager::GetRandomFloat(float min, float max)
{
	float value = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	return min + value * (max - min);
}

void EventManager::SetLastSpace(float lastSpace) {
	sLastSpace = lastSpace;
}