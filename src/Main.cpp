#include "Main.h"
#include "shader/Shader.h"
#include "world/Ground.h"
#include "world/Grass.h"
#include "Camera.h"
#include "Input.h"

#include <iostream>

//KEYBINDINGS
//Escape	- Close / quit
//WASD		- Camera movement
//Arrows	- Camera rotation
//Space		- Camera up
//Shift		- Camera down
//Q/E		- Previous/Next seed
//H			- Toggle heightmap
//P			- Toggle polygon mode
//G			- Toggle grass
//V			- Toggle vsync
//R/F		- Inc/Decrease grass LOD

//Main
namespace Main {
	void init();
	void update(float delta);
	void draw();
}

//Variables
GLFWwindow* window;
const int monitor = 1;
bool vsyncEnabled = true;

//Main
int main() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#define FULLSCREEN 0
#if FULLSCREEN
	int windows;
	GLFWmonitor** monitors = glfwGetMonitors(&windows);

	window = glfwCreateWindow(1920, 1080, "Grass", monitors[monitor], nullptr);
#else
#define BIG_SIZE 1
#if BIG_SIZE
	int width = 1600;
	int height = 900;
#else
	int width = 1280;
	int height = 720;
#endif
	window = glfwCreateWindow(width, height, "Grass", nullptr, nullptr);
#endif

	if (window == nullptr) {
		std::cout << "Failed to open GLFW window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Main::init();

	//Fps counter
	using namespace std::chrono;
	auto prevFPSTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	auto prevTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	int fpsCounter = 0;

	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		auto currTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		fpsCounter++;
		
		auto delta = float(currTime - prevTime);
		prevTime = currTime;
		
		if (currTime - prevFPSTime >= 1000) {
			std::cout << "FPS: " << fpsCounter << std::endl;
			fpsCounter = 0;
			prevFPSTime = currTime;
		}
		
		Main::update(delta);
		Main::draw();

		glfwPollEvents();
	}

	Ground::groundDestroy();
	Grass::grassDestroy();
	Shader::shaderDestroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void Main::init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	Camera::cameraInit(window);

	Shader::groundShaderInit();
	Shader::grassShaderInit();

	Ground::groundInit(window);
	Grass::grassInit(window);

	Input::assignInput(GLFW_KEY_V, []() {
		vsyncEnabled = !vsyncEnabled;
		std::cout << "Vsync: " << vsyncEnabled << std::endl;
		glfwSwapInterval(vsyncEnabled);
	});

	Grass::assignInputs();
	Ground::assignInputs();
}

void Main::update(float delta) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	Shader::refreshShaders();
	Input::updateInputs(window);

	Camera::updateCamera(delta);
	Camera::updateUniforms();
}

void Main::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera::updateGroundUniforms();
	Ground::draw();

	Camera::updateGrassUniforms();
	Grass::draw();

	glfwSwapBuffers(window);
}
