#include "glad/glad.h"

#include "../PerlinNoise.hpp"
#include "Ground.h"
#include "Grass.h"

#include "glm/glm.hpp"
#include <iostream>

namespace Ground {
	GroundData gd;
}

siv::PerlinNoise::seed_type seed = 3; //3,5,16,47
siv::PerlinNoise perlin{seed};

GLFWwindow* groundWindow;

void Ground::groundInit(GLFWwindow* window) {
	groundWindow = window;
	Ground::spawn();

	//Ground buffers
	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindVertexArray(groundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gd), &gd, GL_STATIC_DRAW);

	//Element buffer
	glGenBuffers(1, &groundEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

	//Binding shader attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) offsetof(GroundData, groundVertices));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) offsetof(GroundData, groundNormals));
	glEnableVertexAttribArray(1);
}

void Ground::spawn() {
	//Vertex
	for (int x = 0; x <= size; x++) {
		for (int y = 0; y <= size; y++) {
			const auto fx = float(x);
			const auto fy = float(y);
			const auto fSize = float(size);

			gd.groundVertices[x][y] = glm::vec3(
					(fx / fSize) * 2 - 1,
					perlin.octave2D_01(fx / fSize, fy / fSize, 1),
//					perlin.octave2D_01(fx / 2.0, fy / 2.0, 1),
//					0,
					(fy / fSize) * 2 - 1);
		}
	}

	//Normals
	for (int x = 0; x <= size; x++) {
		for (int y = 0; y <= size; y++) {
			glm::vec3 currPos = gd.groundVertices[x][y];

			glm::vec3 prevXPos(0.0f);
			glm::vec3 nextXPos(0.0f);
			glm::vec3 prevYPos(0.0f);
			glm::vec3 nextYPos(0.0f);

			if (x > 0) prevXPos = gd.groundVertices[x - 1][y] - currPos;
			if (x < size) nextXPos = gd.groundVertices[x + 1][y] - currPos;
			if (y > 0) prevYPos = gd.groundVertices[x][y - 1] - currPos;
			if (y < size) nextYPos = gd.groundVertices[x][y + 1] - currPos;

			glm::vec3 normalX(nextXPos - prevXPos);
			glm::vec3 normalY(nextYPos - prevYPos);

			gd.groundNormals[x][y] = glm::normalize(
					glm::cross(normalX, normalY)
			) * glm::vec3(1, -1, 1);
		}
	}

	//Ground Indices
	int i = 0;
	for (int sizeX = 0; sizeX < size; sizeX++) {
		for (int sizeY = 0; sizeY < size; sizeY++) {
			groundIndices[i] = sizeX + (size + 1) *
									   sizeY;
			groundIndices[i + 1] = groundIndices[i] + 1;
			groundIndices[i + 2] = sizeX + (size + 1) * (sizeY + 1);
			groundIndices[i + 3] = groundIndices[i + 1];
			groundIndices[i + 4] = groundIndices[i + 2];
			groundIndices[i + 5] = sizeX + 1 + (size + 1) * (sizeY + 1);

			i += 6;
		}
	}
}

void Ground::draw() {
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glLineWidth(1.f);

	glBindVertexArray(groundVAO);
	glDrawElements(GL_TRIANGLES, size * size * 6, GL_UNSIGNED_INT, nullptr);
}

bool lastEDown = false;
bool lastQDown = false;

void Ground::update() {
	//Next seed
	if (glfwGetKey(groundWindow, GLFW_KEY_E) == GLFW_PRESS) {
		if (!lastEDown) {
			lastEDown = true;
			perlin.reseed(++seed);
			groundInit(groundWindow);
			Grass::grassInit(groundWindow);
			std::cout << "New seed: " << seed << std::endl;
		}
	} else lastEDown = false;
	//Previous seed
	if (glfwGetKey(groundWindow, GLFW_KEY_Q) == GLFW_PRESS) {
		if (!lastQDown) {
			lastQDown = true;
			perlin.reseed(--seed);
			groundInit(groundWindow);
			std::cout << "New seed: " << seed << std::endl;
		}
	} else lastQDown = false;
}

void Ground::groundDestroy() {
	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, &groundVBO);
}
