#include "glad/glad.h"

#include <iostream>
#include "Grass.h"
#include "Ground.h"

GLFWwindow* grassWindow;
bool grassEnabled = true;

void Grass::grassInit(GLFWwindow* window) {
	grassWindow = window;

	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);

	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground::gd.groundVertices), Ground::gd.groundVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
	glEnableVertexAttribArray(2);
}

void Grass::draw() {
	if (!grassEnabled) return;
	
	glBindVertexArray(grassVAO);
	glDrawArrays(GL_POINTS, 0, (size + 1) * (size + 1));
}

bool lastGDown = false;

void Grass::update() {
	if (glfwGetKey(grassWindow, GLFW_KEY_G) == GLFW_PRESS) {
		if (!lastGDown) {
			lastGDown = true;
			grassEnabled = !grassEnabled;
			std::cout << "Grass " << (grassEnabled ? "enabled" : "disabled") << std::endl;
		}
	} else lastGDown = false;
}

void Grass::grassDestroy() {
	glDeleteBuffers(1, &grassVBO);
	glDeleteBuffers(1, &grassVAO);
}
