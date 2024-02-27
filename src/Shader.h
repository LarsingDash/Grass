#ifndef GRASS_SHADER_MANAGER_H
#define GRASS_SHADER_MANAGER_H

#include <glad/glad.h>

namespace Shader {
	void shaderInit();
	void shaderDestroy();
	void checkShaderErrors(GLuint shaderId);

	extern unsigned int shaderProgram;
}

#endif //GRASS_SHADER_MANAGER_H
