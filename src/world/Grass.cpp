#include "../Input.h"

#include <iostream>
#include "Grass.h"
#include "Ground.h"
#include "../PerlinNoise.hpp"
#include "../shader/Shader.h"
#include "glm/gtc/type_ptr.hpp"

bool grassEnabled = true;
bool polyEnabled = false;
bool windFrozen = false;
bool windActive = true;
float windSize = 0.5f;

float timeOffset = 0.0f;

siv::PerlinNoise::seed_type windSeed = 0;
siv::PerlinNoise perlinX{windSeed};
siv::PerlinNoise perlinY{windSeed};

void Grass::grassInit() {
	Grass::spawn();
	Grass::windData();

	//Grass buffers
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);

	//Assigning empty data
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, Ground::verticesSize + Ground::normalsSize + Ground::windDataSize, nullptr,
				 GL_STATIC_DRAW);

	//Filling SubData
	glBufferSubData(GL_ARRAY_BUFFER, 0, Ground::verticesSize, Ground::verticesData);
	glBufferSubData(GL_ARRAY_BUFFER, Ground::verticesSize, Ground::normalsSize, Ground::normalsData);
	glBufferSubData(GL_ARRAY_BUFFER, Ground::verticesSize + Ground::normalsSize, Ground::windDataSize,
					Ground::windData);

	//Binding pointers
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
						  reinterpret_cast<void*>(static_cast<intptr_t>(Ground::verticesSize + Ground::normalsSize)));
	glEnableVertexAttribArray(3);
}

int layers = 5;
constexpr int maxLayers = 11;
//Amount of layers * (2 triangles * 3 vertices each) - 3 since the top layer has 1 triangle
glm::vec3 grassVertices[(maxLayers + 1) * 2 - 1];
float maxHeight = 0.2f * (50.f / float(Ground::size));

void Grass::spawn() {
	auto fLayer = float(layers);
	float maxOffset = 0.01f * (50.f / float(Ground::size));

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

void Grass::windData() {
	for (int x = 0; x <= Ground::size; x++) {
		for (int y = 0; y <= Ground::size; y++) {
			if (windActive) {
				const auto fx = float(x);
				const auto fy = float(y);

				const auto fSize = float(Ground::size) * windSize;

				Ground::gd.windData[x][y] = glm::vec2(
						perlinX.octave2D_11(fx / fSize + timeOffset, fy / fSize + timeOffset, 1),
						perlinY.octave2D_11(fx / fSize + timeOffset, fy / fSize + timeOffset, 1)
				);
			} else Ground::gd.windData[x][y] = glm::vec2(0);
		}
	}

	Ground::gd.windData[Ground::size][Ground::size] = glm::vec2(-1, -1);
}

void Grass::draw() {
	if (!grassEnabled) return;

	GLint grassColor = glGetUniformLocation(Shader::grassShaderProgram, "grassColor");
	glUniform1i(grassColor, polyEnabled);

	GLint layersLoc = glGetUniformLocation(Shader::grassShaderProgram, "layers");
	glUniform1i(layersLoc, layers);

	GLint gridSize = glGetUniformLocation(Shader::grassShaderProgram, "gridSize");
	glUniform1i(gridSize, Ground::size);

	GLint maxHeightLoc = glGetUniformLocation(Shader::grassShaderProgram, "maxHeight");
	glUniform1f(maxHeightLoc, maxHeight);

	GLint points = glGetUniformLocation(Shader::grassShaderProgram, "pointsRaw");
	glUniform3fv(points, (maxLayers + 1) * 2 - 1, glm::value_ptr(grassVertices[0]));

	glBindVertexArray(grassVAO);
	glDrawArrays(GL_POINTS, 0, (Ground::size + 1) * (Ground::size + 1) - 1); //minus bozo
}

void Grass::update(float delta) {
	if (!windFrozen) timeOffset += delta / 10000.0f;
	Grass::windData();

	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, Ground::verticesSize + Ground::normalsSize + Ground::windDataSize, nullptr,
				 GL_STATIC_DRAW);

	//Filling SubData
	glBufferSubData(GL_ARRAY_BUFFER, 0, Ground::verticesSize, Ground::verticesData);
	glBufferSubData(GL_ARRAY_BUFFER, Ground::verticesSize, Ground::normalsSize, Ground::normalsData);
	glBufferSubData(GL_ARRAY_BUFFER, Ground::verticesSize + Ground::normalsSize, Ground::windDataSize,
					Ground::windData);
}

void Grass::assignInputs() {
	Input::assignInput(GLFW_KEY_P, []() {
		polyEnabled = !polyEnabled;
		glPolygonMode(GL_FRONT_AND_BACK, polyEnabled ? GL_LINE : GL_FILL);
		std::cout << "Poly: " << polyEnabled << std::endl;
	});
	Input::assignInput(GLFW_KEY_G, []() {
		grassEnabled = !grassEnabled;
		std::cout << "Grass: " << grassEnabled << std::endl;
	});
	Input::assignInput(GLFW_KEY_Z, []() {
		layers = glm::max(1, layers - 1);
		Grass::spawn();
		std::cout << "LOD: " << layers << std::endl;
	});
	Input::assignInput(GLFW_KEY_X, []() {
		layers = glm::min(maxLayers - 1, layers + 1);
		Grass::spawn();
		std::cout << "LOD: " << layers << std::endl;
	});
	Input::assignInput(GLFW_KEY_F, []() {
		windFrozen = !windFrozen;
		std::cout << "WindFrozen: " << windFrozen << std::endl;
	});
	Input::assignInput(GLFW_KEY_R, []() {
		windActive = !windActive;
		std::cout << "WindActive: " << windActive << std::endl;
	});
	Input::assignInput(GLFW_KEY_COMMA, []() {
		windSize = glm::max(0.00f, windSize - 0.02f);
		std::cout << "WindSize: " << windSize << std::endl;
	});
	Input::assignInput(GLFW_KEY_PERIOD, []() {
		windSize = glm::min(1.5f, windSize + 0.02f);
		std::cout << "WindSize: " << windSize << std::endl;
	});
}

void Grass::grassDestroy() {
	glDeleteBuffers(1, &grassVBO);
	glDeleteBuffers(1, &grassVAO);
}
