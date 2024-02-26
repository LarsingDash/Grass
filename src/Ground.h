#ifndef GRASS_GROUND_H
#define GRASS_GROUND_H

#include <glm/vec3.hpp>
#include <vector>

namespace Ground {
	void groundInit();
	void groundDestroy();
	void draw();
	void spawn();

	static unsigned int groundVBO, groundVAO, groundEBO;
}

#endif //GRASS_GROUND_H
