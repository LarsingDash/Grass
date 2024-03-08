#ifndef GRASS_GRASS_H
#define GRASS_GRASS_H

#include "../Main.h"

namespace Grass {
	void grassInit(GLFWwindow* window);
	void spawn();
	void draw();
	void assignInputs();
	void grassDestroy();
	
	static unsigned int grassVBO, grassVAO;
}

#endif //GRASS_GRASS_H
