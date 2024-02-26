#include <iostream>

#include "ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Main
void init();
void update();
void draw();

//Variables
GLFWwindow* window;
const int monitor = 1;

//Ground
float groundVertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
};
unsigned int groundVBO, groundVAO;

//Main
int main() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#define FULLSCREEN 1
#if FULLSCREEN
	int windows;
	GLFWmonitor** monitors = glfwGetMonitors(&windows);

	window = glfwCreateWindow(1920, 1080, "Grass", monitors[monitor], nullptr);
	glfwSwapInterval(1);
#else
	window = glfwCreateWindow(1920, 1080, "Grass", nullptr, nullptr);
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

	init();

	while (!glfwWindowShouldClose(window)) {
		update();
		draw();

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, &groundVBO);
	ShaderManager::shaderDestroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void init() {
	ShaderManager::shaderInit();

	//Ground buffers
	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindVertexArray(groundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

	//Binding shader attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
	glEnableVertexAttribArray(0);
}

void update() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void draw() {
	//GL enable (move to init?)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(groundVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(window);
}
