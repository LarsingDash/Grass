#include "Shader.h"

#include <fstream>
#include <iostream>

namespace Shader {
	unsigned int groundShaderProgram = 0;
	unsigned int grassShaderProgram = 0;
}

const char* groundVert = "shader/files/ground.vert";
const char* groundFrag = "shader/files/ground.frag";
const char* grassVert = "shader/files/grass.vert";
const char* grassGeom = "shader/files/grass.geom";
const char* grassFrag = "shader/files/grass.frag";

unsigned int groundVertShader;
unsigned int groundFragShader;
unsigned int grassVertShader;
unsigned int grassGeomShader;
unsigned int grassFragShader;

void Shader::groundShaderInit() {
	groundShaderProgram = glCreateProgram();

	loadShader(groundVert, groundShaderProgram, GL_VERTEX_SHADER, groundVertShader);
	loadShader(groundFrag, groundShaderProgram, GL_FRAGMENT_SHADER, groundFragShader);
}

void Shader::grassShaderInit() {
	grassShaderProgram = glCreateProgram();

	loadShader(grassVert, grassShaderProgram, GL_VERTEX_SHADER, grassVertShader);
	loadShader(grassGeom, grassShaderProgram, GL_GEOMETRY_SHADER, grassGeomShader);
	loadShader(grassFrag, grassShaderProgram, GL_FRAGMENT_SHADER, grassFragShader);
}

void Shader::loadShader(const char* path, unsigned int program, unsigned int type, unsigned int& shader) {
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

//Text being way too long
using fileTime = std::filesystem::file_time_type;

fileTime lastWrite(const char* path) {
	return std::filesystem::last_write_time(path);
}

fileTime prevGroundVertTime = lastWrite(groundVert);
fileTime prevGroundFragTime = lastWrite(groundFrag);
fileTime prevGrassVertTime = lastWrite(grassVert);
fileTime prevGrassGeomTime = lastWrite(grassGeom);
fileTime prevGrassFragTime = lastWrite(grassFrag);

void Shader::refreshShaders() {
	fileTime groundVertTime = lastWrite(groundVert);
	fileTime groundFragTime = lastWrite(groundFrag);
	fileTime grassVertTime = lastWrite(grassVert);
	fileTime grassGeomTime = lastWrite(grassGeom);
	fileTime grassFragTime = lastWrite(grassFrag);

	_refreshShader(prevGroundVertTime, groundVertTime, groundShaderProgram, groundVertShader, groundVert,
				   GL_VERTEX_SHADER);
	_refreshShader(prevGroundFragTime, groundFragTime, groundShaderProgram, groundFragShader, groundFrag,
				   GL_FRAGMENT_SHADER);
	_refreshShader(prevGrassVertTime, grassVertTime, grassShaderProgram, grassVertShader, grassVert, GL_VERTEX_SHADER);
	_refreshShader(prevGrassGeomTime, grassGeomTime, grassShaderProgram, grassGeomShader, grassGeom,
				   GL_GEOMETRY_SHADER);
	_refreshShader(prevGrassFragTime, grassFragTime, grassShaderProgram, grassFragShader, grassFrag,
				   GL_FRAGMENT_SHADER);
}

void Shader::_refreshShader(std::filesystem::file_time_type& prevTime, std::filesystem::file_time_type& time,
							unsigned int program, unsigned int shader, const char* path, unsigned int type) {
	if (prevTime != time) {
		prevTime = time;

		std::cout << "Updated " << path << std::endl;

		glDetachShader(program, shader);
		glDeleteShader(shader);

		Shader::loadShader(path, program, type, shader);
	}
}

void Shader::shaderDestroy() {
	glDeleteProgram(groundShaderProgram);
}

//Helper
void Shader::checkShaderErrors(unsigned int shaderId) {
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
