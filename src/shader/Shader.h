#ifndef GRASS_SHADER_MANAGER_H
#define GRASS_SHADER_MANAGER_H

#include "glad/glad.h"

namespace Shader {
	void groundShaderInit();
	void loadShader(const char* path, unsigned int program, GLenum type, unsigned int& shader);
	void refreshShaders();
	void shaderDestroy();
	void checkShaderErrors(GLuint shaderId);

	extern unsigned int groundShaderProgram;
}

#endif //GRASS_SHADER_MANAGER_H
