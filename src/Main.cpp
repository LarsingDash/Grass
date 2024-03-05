#include "shader/Shader.h"
#include "world/Ground.h"
#include "world/Grass.h"
#include "Camera.h"

#include <iostream>

//Main
namespace Main {
	void init();
	void update();
	void draw();
}

//Variables
GLFWwindow* window;
const int monitor = 1;

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
	glfwSwapInterval(1);
#else
	window = glfwCreateWindow(1280, 720, "Grass", nullptr, nullptr);
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

	while (!glfwWindowShouldClose(window)) {
		Main::update();
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
}

void Main::update() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	Shader::refreshShaders();
	
	Camera::updateCamera();
	Camera::updateUniforms();
	
	Ground::update();
	Grass::update();
}

void Main::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Camera::updateGroundUniforms();
	Ground::draw();
	
	Camera::updateGrassUniforms();
	Grass::draw();

	glfwSwapBuffers(window);
}
