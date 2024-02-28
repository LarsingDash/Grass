#include <glad/glad.h>

#include "PerlinNoise.hpp"
#include "Ground.h"

#include <glm/glm.hpp>
#include <iostream>

constexpr int size = 25;
glm::vec3 groundVertices[size + 1][size + 1];
GLuint groundIndices[size * size * 6];

siv::PerlinNoise::seed_type seed = 16; //3,5,16,47
siv::PerlinNoise perlin{seed};

GLFWwindow* gWindow;

void Ground::groundInit(GLFWwindow* window) {
	gWindow = window;
	Ground::spawn();

	//Ground buffers
	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindVertexArray(groundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

	//Element buffer
	glGenBuffers(1, &groundEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

	//Binding shader attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
	glEnableVertexAttribArray(0);
}

void Ground::groundDestroy() {
	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, &groundVBO);
}

void Ground::spawn() {
	//Ground Vertices
	for (int x = 0; x <= size; x++) {
		for (int y = 0; y <= size; y++) {
			groundVertices[x][y] = glm::vec3(
					(float(x) / float(size)) * 2 - 1,
					perlin.octave2D_01(float(x) / 20.0, float(y) / 20.0, 1),
					(float(y) / float(size)) * 2 - 1);
		}
	}

	//GroundIndices
	int i = 0;
	for (int sizeX = 0; sizeX < size; sizeX++) {
		for (int sizeY = 0; sizeY < size; sizeY++) {
			groundIndices[i] = sizeX + (size + 1) * sizeY;
			groundIndices[i + 1] = groundIndices[i] + 1;
			groundIndices[i + 2] = sizeX + (size + 1) * (sizeY + 1);
			groundIndices[i + 3] = groundIndices[i + 1];
			groundIndices[i + 4] = groundIndices[i + 2];
			groundIndices[i + 5] = sizeX + 1 + (size + 1) * (sizeY + 1);

			i += 6;
		}
	}
}

bool lastDown = false;

void Ground::draw() {
	//Next seed
	if (glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_PRESS) {
		if (!lastDown) {
			lastDown = true;
			perlin.reseed(++seed);
			groundInit(gWindow);
		}
	} else lastDown = false;

	//Drawing
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glLineWidth(1.f);
	glBindVertexArray(groundVAO);
	glDrawElements(GL_TRIANGLES, size * size * 6, GL_UNSIGNED_INT, nullptr);
}
