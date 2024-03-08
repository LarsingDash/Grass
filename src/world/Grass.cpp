#include "glad/glad.h"

#include <iostream>
#include "Grass.h"
#include "Ground.h"
#include "../shader/Shader.h"
#include "glm/gtc/type_ptr.hpp"

GLFWwindow* grassWindow;
bool grassEnabled = true;
bool polyEnabled = false;

void Grass::grassInit(GLFWwindow* window) {
	grassWindow = window;

	Grass::spawn();

	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);

	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground::gd.groundVertices), Ground::gd.groundVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
	glEnableVertexAttribArray(2);
}

int layers = 3;
constexpr int maxLayers = 7;
//Amount of layers * (2 triangles * 3 vertices each) - 3 since the top layer has 1 triangle
glm::vec3 grassVertices[(maxLayers + 1) * 2 - 1];
//int grassIndices[maxLayers * 6 - 3];
constexpr float maxHeight = 0.2f * (50.f / float(size));

void Grass::spawn() {
	auto fLayer = float(layers);
	float maxOffset = 0.01f * (50.f / float(size));

	//Generate vertices: 2 per layer + top
	int vertI = 0;
	for (int l = 0; l < layers; l++) {
		float x = (1 - float(l) / fLayer) * maxOffset;
		float y = float(l) / fLayer * maxHeight;

		grassVertices[vertI] = glm::vec3(-x, y, 0);
		grassVertices[vertI + 1] = glm::vec3(x, y, 0);

		vertI += 2;
	}
	grassVertices[vertI] = glm::vec3(0, maxHeight, 0);
}

void Grass::draw() {
	if (!grassEnabled) return;

	GLint grassColor = glGetUniformLocation(Shader::grassShaderProgram, "grassColor");
	glUniform1i(grassColor, polyEnabled);

	GLint layersLoc = glGetUniformLocation(Shader::grassShaderProgram, "layers");
	glUniform1i(layersLoc, layers);

	GLint gridSize = glGetUniformLocation(Shader::grassShaderProgram, "gridSize");
	glUniform1i(gridSize, size);

	GLint points = glGetUniformLocation(Shader::grassShaderProgram, "points");
	glUniform3fv(points, (maxLayers + 1) * 2 - 1, glm::value_ptr(grassVertices[0]));

	glBindVertexArray(grassVAO);
	glDrawArrays(GL_POINTS, 0, (size + 1) * (size + 1));
}

bool lastGDown = false;
bool lastPDown = false;
bool lastRDown = false;
bool lastFDown = false;

void Grass::update() {
	if (glfwGetKey(grassWindow, GLFW_KEY_P) == GLFW_PRESS) {
		if (!lastPDown) {
			lastPDown = true;
			polyEnabled = !polyEnabled;
			glPolygonMode(GL_FRONT_AND_BACK, polyEnabled ? GL_LINE : GL_FILL);
			std::cout << "Poly: " << (polyEnabled ? "enabled" : "disabled") << std::endl;
		}
	} else lastPDown = false;
	if (glfwGetKey(grassWindow, GLFW_KEY_G) == GLFW_PRESS) {
		if (!lastGDown) {
			lastGDown = true;
			grassEnabled = !grassEnabled;
			std::cout << "Grass: " << (grassEnabled ? "enabled" : "disabled") << std::endl;
		}
	} else lastGDown = false;
	if (glfwGetKey(grassWindow, GLFW_KEY_R) == GLFW_PRESS) {
		if (!lastRDown) {
			lastRDown = true;
			layers++;
			Grass::spawn();
			std::cout << "LOD: " << layers << std::endl;
		}
	} else lastRDown = false;
	if (glfwGetKey(grassWindow, GLFW_KEY_F) == GLFW_PRESS) {
		if (!lastFDown) {
			lastFDown = true;
			layers--;
			Grass::spawn();
			std::cout << "LOD: " << layers << std::endl;
		}
	} else lastFDown = false;
}

void Grass::grassDestroy() {
	glDeleteBuffers(1, &grassVBO);
	glDeleteBuffers(1, &grassVAO);
}
