#ifndef GRASS_GRASS_H
#define GRASS_GRASS_H

#include "GLFW/glfw3.h"

namespace Grass {
	void grassInit(GLFWwindow* window);
	void draw();
	void update();
	void grassDestroy();
	
	static unsigned int grassVBO, grassVAO;
}

#endif //GRASS_GRASS_H
