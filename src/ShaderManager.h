#ifndef GRASS_SHADER_MANAGER_H
#define GRASS_SHADER_MANAGER_H

#include <fstream>
#include <iostream>

#include <glad/glad.h>

class ShaderManager {
public:
	static void shaderInit();
	static void shaderDestroy();

private:
	static void checkShaderErrors(GLuint shaderId);

	static unsigned int shaderProgram;
};

#endif //GRASS_SHADER_MANAGER_H
