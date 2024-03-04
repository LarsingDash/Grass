#ifndef GRASS_GROUND_H
#define GRASS_GROUND_H

#include "GLFW/glfw3.h"

#include "glm/vec3.hpp"
#include <vector>

constexpr int size = 50;
struct GroundData {
	glm::vec3 groundVertices[size + 1][size + 1];
	glm::vec3 groundNormals[size + 1][size + 1];
};

namespace Ground {
	void groundInit(GLFWwindow* window);
	void groundDestroy();
	void draw();
	void spawn();
	
	extern GroundData gd;
	static unsigned int groundVBO, groundVAO, groundEBO;
}

#endif //GRASS_GROUND_H
