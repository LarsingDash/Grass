#include "Shader.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace Shader {
	unsigned int groundShaderProgram = 0;
}

const char* groundVert = "shader/files/ground.vert";
const char* groundFrag = "shader/files/ground.frag";
unsigned int groundVertShader;
unsigned int groundFragShader;

void Shader::groundShaderInit() {
	groundShaderProgram = glCreateProgram();

	//Shaders
	loadShader(groundVert, groundShaderProgram, GL_VERTEX_SHADER, groundVertShader);
	loadShader(groundFrag, groundShaderProgram, GL_FRAGMENT_SHADER, groundFragShader);

	//Program
	glLinkProgram(groundShaderProgram);
	glUseProgram(groundShaderProgram);
}

void Shader::loadShader(const char* path, unsigned int program, GLenum type, unsigned int& shader) {
	std::ifstream shaderFile(path);
	std::string shaderData((std::istreambuf_iterator<char>(shaderFile)),
						   std::istreambuf_iterator<char>());
	const char* constShaderData = shaderData.c_str();

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &constShaderData, nullptr);
	glCompileShader(shader);
	checkShaderErrors(shader);

	glAttachShader(program, shader);
	glDeleteShader(shader);
}

std::filesystem::file_time_type prevGroundVertTime = std::filesystem::last_write_time(groundVert);
std::filesystem::file_time_type prevGroundFragTime = std::filesystem::last_write_time(groundFrag);

void Shader::refreshShaders() {
	std::filesystem::file_time_type groundVertTime = std::filesystem::last_write_time(groundVert);
	std::filesystem::file_time_type groundFragTime = std::filesystem::last_write_time(groundFrag);
	
	if (groundVertTime != prevGroundVertTime) {
		prevGroundVertTime = groundVertTime;
		std::cout << "Updated groundVertShader" << std::endl;

		glDetachShader(groundShaderProgram, groundVertShader);
		glDeleteShader(groundVertShader);

		loadShader(groundVert, groundShaderProgram, GL_VERTEX_SHADER, groundVertShader);

		glLinkProgram(groundShaderProgram);
		glUseProgram(groundShaderProgram);
	}
	if (groundFragTime != prevGroundFragTime) {
		prevGroundFragTime = groundFragTime;
		std::cout << "Updated groundFragShader" << std::endl;

		glDetachShader(groundShaderProgram, groundFragShader);
		glDeleteShader(groundFragShader);

		loadShader(groundFrag, groundShaderProgram, GL_FRAGMENT_SHADER, groundFragShader);

		glLinkProgram(groundShaderProgram);
		glUseProgram(groundShaderProgram);
	}
}

void Shader::shaderDestroy() {
	glDeleteProgram(groundShaderProgram);
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
