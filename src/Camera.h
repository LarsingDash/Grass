#ifndef GRASS_CAMERA_H
#define GRASS_CAMERA_H

#include "Main.h"

namespace Camera {
	void cameraInit(GLFWwindow* window);

	void updateCamera(float delta);
	void updateUniforms();
	
	void updateGroundUniforms();
	void updateGrassUniforms();

	static GLFWwindow* windowC;
}

#endif //GRASS_CAMERA_H
