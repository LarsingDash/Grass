#include "Shader.h"

#include <fstream>
#include <iostream>

namespace Shader {
	unsigned int shaderProgram = 0;
}

void Shader::shaderInit() {
	//Shaders	
	std::ifstream vertexShaderFile("shader/ground.vert");
	std::string vertexShaderData((std::istreambuf_iterator<char>(vertexShaderFile)),
								 std::istreambuf_iterator<char>());
	const char* constVertexShaderData = vertexShaderData.c_str();

	std::ifstream fragShaderFile("shader/ground.frag");
	std::string fragShaderData((std::istreambuf_iterator<char>(fragShaderFile)), std::istreambuf_iterator<char>());
	const char* constFragShaderData = fragShaderData.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &constVertexShaderData, nullptr);
	glCompileShader(vertexShader);
	checkShaderErrors(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &constFragShaderData, nullptr);
	glCompileShader(fragmentShader);
	checkShaderErrors(fragmentShader);

	//Program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);
}

void Shader::shaderDestroy() {
	glDeleteProgram(shaderProgram);
}

//Helper
void Shader::checkShaderErrors(GLuint shaderId) {
	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		int length, charsWritten;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* infoLog = new char[length + 1];
		memset(infoLog, 0, length + 1);
		glGetShaderInfoLog(shaderId, length, &charsWritten, infoLog);
		std::cout << "Error compiling shader:\n" << infoLog << std::endl;
		delete[] infoLog;
	}
}
