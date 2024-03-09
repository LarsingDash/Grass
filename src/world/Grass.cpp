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

float timeOffset = 0.0f;

siv::PerlinNoise::seed_type windSeed = 0;
siv::PerlinNoise perlinX{windSeed};
siv::PerlinNoise perlinY{windSeed};

void Grass::grassInit() {
	Grass::spawn();
	Grass::windData();

	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);

	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground::gd), &Ground::gd, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) offsetof(GroundData, groundVertices));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) offsetof(GroundData, windData));
	glEnableVertexAttribArray(3);
}

int layers = 5;
constexpr int maxLayers = 10;
//Amount of layers * (2 triangles * 3 vertices each) - 3 since the top layer has 1 triangle
glm::vec3 grassVertices[(maxLayers + 1) * 2 - 1];
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

void Grass::windData() {
	for (int x = 0; x <= size; x++) {
		for (int y = 0; y <= size; y++) {
			if (windActive) {
				const auto fx = float(x);
				const auto fy = float(y);

				const auto fSize = float(size) * 1.5;

				Ground::gd.windData[x][y] = glm::vec2(
						perlinX.octave2D_11(fx / fSize + timeOffset, fy / fSize + timeOffset, 1),
						perlinY.octave2D_11(fx / fSize + timeOffset, fy / fSize + timeOffset, 1)
				);
			} else Ground::gd.windData[x][y] = glm::vec2(0);
		}
	}
}

void Grass::draw() {
	if (!grassEnabled) return;

	GLint grassColor = glGetUniformLocation(Shader::grassShaderProgram, "grassColor");
	glUniform1i(grassColor, polyEnabled);

	GLint layersLoc = glGetUniformLocation(Shader::grassShaderProgram, "layers");
	glUniform1i(layersLoc, layers);

	GLint gridSize = glGetUniformLocation(Shader::grassShaderProgram, "gridSize");
	glUniform1i(gridSize, size);

	GLint maxHeightLoc = glGetUniformLocation(Shader::grassShaderProgram, "maxHeight");
	glUniform1f(maxHeightLoc, maxHeight);

	GLint points = glGetUniformLocation(Shader::grassShaderProgram, "pointsRaw");
	glUniform3fv(points, (maxLayers + 1) * 2 - 1, glm::value_ptr(grassVertices[0]));

	glBindVertexArray(grassVAO);
	glDrawArrays(GL_POINTS, 0, (size + 1) * (size + 1));
}

void Grass::update(float delta) {
	if (!windFrozen) timeOffset += delta / 10000;
	Grass::windData();

	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(data, &Ground::gd, sizeof(Ground::gd));
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Grass::assignInputs() {
	Input::assignInput(GLFW_KEY_P, []() {
		polyEnabled = !polyEnabled;
		glPolygonMode(GL_FRONT_AND_BACK, polyEnabled ? GL_LINE : GL_FILL);
		std::cout << "Poly: " << (polyEnabled ? "enabled" : "disabled") << std::endl;
	});
	Input::assignInput(GLFW_KEY_G, []() {
		grassEnabled = !grassEnabled;
		std::cout << "Grass: " << (grassEnabled ? "enabled" : "disabled") << std::endl;
	});
	Input::assignInput(GLFW_KEY_Z, []() {
		layers = glm::max(1, layers - 1);
		Grass::spawn();
		std::cout << "LOD: " << layers << std::endl;
	});
	Input::assignInput(GLFW_KEY_X, []() {
		layers = glm::min(maxLayers, layers + 1);
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
}

void Grass::grassDestroy() {
	glDeleteBuffers(1, &grassVBO);
	glDeleteBuffers(1, &grassVAO);
}
