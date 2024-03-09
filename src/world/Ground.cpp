#include "../Input.h"
#include "../PerlinNoise.hpp"
#include "Ground.h"
#include "Grass.h"

#include "glm/glm.hpp"
#include <iostream>

int size = 100;
namespace Ground {

	GroundData gd;
	int verticesSize;
	int normalsSize;
	int windDataSize;

	glm::vec3* verticesData = new glm::vec3[(size + 1) * (size + 1)];
	glm::vec3* normalsData = new glm::vec3[(size + 1) * (size + 1)];
	glm::vec2* windData = new glm::vec2[(size + 1) * (size + 1)];
}

siv::PerlinNoise::seed_type groundSeed = 3; //3,5,16,47
siv::PerlinNoise perlin{groundSeed};

bool isHeightEnabled = true;

void Ground::groundInit() {
	// Initialize pointers array
	auto** vertices = new glm::vec3* [size + 1];
	auto** normals = new glm::vec3* [size + 1];
	auto** wind = new glm::vec2* [size + 1];
	for (int i = 0; i <= size; i++) {
		vertices[i] = &verticesData[i * (size + 1)];
		normals[i] = &normalsData[i * (size + 1)];
		wind[i] = &windData[i * (size + 1)];
	}

	//Assign to gd
	gd = GroundData{
			.groundVertices = vertices,
			.groundNormals = normals,
			.windData = wind,
	};

	//Sizes
	verticesSize = int(sizeof(glm::vec3)) * (size + 1) * (size + 1);
	normalsSize = int(sizeof(glm::vec3)) * (size + 1) * (size + 1);
	windDataSize = int(sizeof(glm::vec2)) * (size + 1) * (size + 1);

	//Indices
	groundIndices = new GLuint[(size * size * 6)];

	Ground::spawn();

	//Ground buffers
	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindVertexArray(groundVAO);

	//Assigning empty data
	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize + normalsSize + windDataSize, nullptr, GL_STATIC_DRAW);

	//Filling SubData
	glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, verticesData);
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize, normalsSize, normalsData);
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, windDataSize, windData);

	//Element buffer
	glGenBuffers(1, &groundEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, int(sizeof(GLuint)) * size * size * 6, groundIndices, GL_STATIC_DRAW);

	//Binding pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr); // Assuming vertices are at offset 0
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
						  reinterpret_cast<void*>(static_cast<intptr_t>(verticesSize)));
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
					isHeightEnabled ? perlin.octave2D_11(fx / fSize, fy / fSize, 1) / 2.0f : 0,
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

void Ground::draw() {
	glBindVertexArray(groundVAO);
	glDrawElements(GL_TRIANGLES, size * size * 6, GL_UNSIGNED_INT, nullptr);
}

void Ground::assignInputs() {
	Input::assignInput(GLFW_KEY_E, []() {
		perlin.reseed(++groundSeed);
		groundInit();
		Grass::grassInit();
		std::cout << "New seed: " << groundSeed << std::endl;
	});
	Input::assignInput(GLFW_KEY_Q, []() {
		perlin.reseed(--groundSeed);
		groundInit();
		Grass::grassInit();
		std::cout << "New seed: " << groundSeed << std::endl;
	});
	Input::assignInput(GLFW_KEY_H, []() {
		isHeightEnabled = !isHeightEnabled;
		groundInit();
		Grass::grassInit();
		std::cout << "Heightmap: " << isHeightEnabled << std::endl;
	});
	Input::assignInput(GLFW_KEY_MINUS, []() {
		size -= 10;
		groundInit();
		Grass::grassInit();
		std::cout << "Size: " << size << std::endl;
	});
	Input::assignInput(GLFW_KEY_EQUAL, []() {
		size += 10;
		groundInit();
		Grass::grassInit();
		std::cout << "Size: " << size << std::endl;
	});
}

void Ground::groundDestroy() {
	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, &groundVBO);
}
