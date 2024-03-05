#ifndef GRASS_SHADER_MANAGER_H
#define GRASS_SHADER_MANAGER_H

#include "glad/glad.h"
#include <filesystem>

namespace Shader {
	void groundShaderInit();
	void grassShaderInit();
	
	void loadShader(const char* path, unsigned int program, GLenum type, unsigned int& shader);
	
	void refreshShaders();
	void _refreshShader(std::filesystem::file_time_type &prevTime, std::filesystem::file_time_type &time, unsigned int program, unsigned int shader, const char *path, GLenum type);
	
	void shaderDestroy();
	void checkShaderErrors(GLuint shaderId);

	extern unsigned int groundShaderProgram;
	extern unsigned int grassShaderProgram;
}

#endif //GRASS_SHADER_MANAGER_H
