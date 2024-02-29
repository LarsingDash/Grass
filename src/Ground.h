#ifndef GRASS_GROUND_H
#define GRASS_GROUND_H

#include "GLFW/glfw3.h"

#include <glm/vec3.hpp>
#include <vector>

namespace Ground {
	void groundInit(GLFWwindow* window);
	void groundDestroy();
	void draw();
	void spawn();

	static unsigned int groundVBO, groundVAO, groundEBO;
}

constexpr int size = 25;
struct GroundData {
	glm::vec3 groundVertices[size + 1][size + 1];
	glm::vec3 groundNormals[size + 1][size + 1];
};

#endif //GRASS_GROUND_H
