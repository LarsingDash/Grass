#ifndef GRASS_GROUND_H
#define GRASS_GROUND_H

#include "../Main.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <vector>

constexpr int size = 100;
struct GroundData {
	glm::vec3 **groundVertices;
	glm::vec3 **groundNormals;
	glm::vec2 **windData;
};

namespace Ground {
	void groundInit();
	void assignInputs();
	void draw();
	void spawn();
	void groundDestroy();

	extern GroundData gd;
	extern int verticesSize;
	extern int normalsSize;
	extern int windDataSize;
	extern glm::vec3* verticesData;
	extern glm::vec3* normalsData;
	extern glm::vec2* windData;
	
	static GLuint groundIndices[size * size * 6];
	static unsigned int groundVBO, groundVAO, groundEBO;
}

#endif //GRASS_GROUND_H
