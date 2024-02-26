#ifndef GRASS_SHADER_MANAGER_H
#define GRASS_SHADER_MANAGER_H

#include <glad/glad.h>

namespace ShaderManager {
	void shaderInit();
	void shaderDestroy();
	void checkShaderErrors(GLuint shaderId);

	static unsigned int shaderProgram;
};

#endif //GRASS_SHADER_MANAGER_H
