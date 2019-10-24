#pragma once

#include <vector>

struct GLFWwindow;

struct WindowResolution {
	int width;
	int height;
};

class EventManager
{
public:
	static void Initialize();
	static void Shutdown();

	static void Update();

	static float GetFrameTime();
	static bool ExitRequested();

	static GLFWwindow* GetWindow();

	static WindowResolution GetWindowResolution();

	static float GetMouseMotionX();
	static float GetMouseMotionY();

	static float GetMinKeyWait();
	static float GetLastSpace();

	static void SetLastSpace(float lastSpace);

	static void EnableMouseCursor();
	static void DisableMouseCursor();
	static float GetRandomFloat(float min, float max);
private:
	// Time
	static double sLastFrameTime;
	static float sFrameTime;

	// Key Delays
	const static float sMinKeyWait;
	static float sLastSpace;

	// Window Resolution
	static std::vector<WindowResolution> sPossibleResolutions;
	static WindowResolution sWindowResolution;
	static int sCurrentResolution;
	static void ToggleResolution();
	static void ReloadWindow();

	// Mouse
	static double sLastMousePositionX;
	static float  sMouseDeltaX;
	static double sLastMousePositionY;
	static float  sMouseDeltaY;

	// Window
	static GLFWwindow* spWindow;
};
