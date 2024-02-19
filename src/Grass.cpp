#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Main
void init();
void update();
void draw();

//Helper
void checkShaderErrors(GLuint shaderId);

//Variables
GLFWwindow* window;
const int monitor = 0;

//Ground
float groundVertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
};
unsigned int groundVBO, groundVAO;

//Shaders
unsigned int shaderProgram;

//Main
int main() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#define FULLSCREEN 0
#if FULLSCREEN
		int windows;
		GLFWmonitor** monitors = glfwGetMonitors(&windows);

		window = glfwCreateWindow(1920, 1080, "Grass", monitors[monitor], nullptr);
		glfwSwapInterval(1);
#else
		window = glfwCreateWindow(1920, 1080, "Grass", nullptr, nullptr);
#endif
		
	if (window == nullptr) {
		std::cout << "Failed to open GLFW window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	init();

	while (!glfwWindowShouldClose(window)) {
		update();
		draw();

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, &groundVBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void init() {
	//Shaders	
	std::ifstream vertexShaderFile("shader/simple.vs");
	std::string vertexShaderData((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
	const char* constVertexShaderData = vertexShaderData.c_str();

	std::ifstream fragShaderFile("shader/simple.fs");
	std::cout << fragShaderFile.good() << std::endl;
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

	//Ground buffers
	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glBindVertexArray(groundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

	//Binding shader attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
	glEnableVertexAttribArray(0);
}

void update() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void draw() {
	//GL enable (move to init?)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(groundVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(window);
}

//Helper
void checkShaderErrors(GLuint shaderId) {
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