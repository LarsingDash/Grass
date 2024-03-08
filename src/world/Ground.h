#ifndef GRASS_GROUND_H
#define GRASS_GROUND_H

#include "../Main.h"

#include "glm/vec3.hpp"
#include <vector>

constexpr int size = 50;
struct GroundData {
	glm::vec3 groundVertices[size + 1][size + 1];
	glm::vec3 groundNormals[size + 1][size + 1];
};

namespace Ground {
	void groundInit();
	void assignInputs();
	void draw();
	void spawn();
	void groundDestroy();

	extern GroundData gd;
	static GLuint groundIndices[size * size * 6];
	static unsigned int groundVBO, groundVAO, groundEBO;
}

#endif //GRASS_GROUND_H
