#ifndef GRASS_GROUND_H
#define GRASS_GROUND_H

#include <glm/vec3.hpp>
#include <vector>

namespace Ground {
	void groundInit();
	void groundDestroy();
	void spawn();

	static unsigned int groundVBO, groundVAO, groundEBO;
	static glm::vec3 groundVertices[] = {
			glm::vec3(-1.f, -1.f, 0.0f),
			glm::vec3(1.0f, -1.f, 0.0f),
			glm::vec3(-1.0f, 1.0f, 0.0f),
			glm::vec3(1.f, 1.0f, 0.0f),
	};
}

#endif //GRASS_GROUND_H
