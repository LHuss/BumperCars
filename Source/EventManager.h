#pragma once

#include <vector>
#include <map>

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
	static bool CanUseKey(int key);

	static void EnableMouseCursor();
	static void DisableMouseCursor();
	static float GetRandomFloat(float min, float max);
private:
	// Time
	static double sLastFrameTime;
	static float sFrameTime;

	// Key Delays
	const static float sMinKeyWait;
	static std::map<int, float> sKeyMap;

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
