﻿#ifndef GRASS_CAMERA_H
#define GRASS_CAMERA_H

#include "GLFW/glfw3.h"

namespace Camera {
	void cameraInit(GLFWwindow* window);

	void updateCamera();
	void updateUniforms();
	
	void updateGroundUniforms();
	void updateGrassUniforms();

	static GLFWwindow* windowC;
}

#endif //GRASS_CAMERA_H
