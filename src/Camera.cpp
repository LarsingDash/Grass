#include "shader/Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 eye = glm::vec3(0.0f, 0.5f, 2.0f);
glm::vec3 center = glm::vec3(0.0f, 0.25f, 1.0f);

constexpr float baseSpeed = 500.f; //Increase in number = decrease in speed
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 forward = glm::normalize(center - eye);
glm::vec3 right = glm::normalize(glm::cross(forward, up));

glm::vec3 rotateAroundAxis(const glm::vec3& pointToRotate, const glm::vec3& axis, float angle) {
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
	return (rotationMatrix * glm::vec4(pointToRotate, 1.0f));
}

void Camera::cameraInit(GLFWwindow* window) {
	windowC = window;
	forward = glm::normalize(center - eye);
}

void Camera::updateCamera(float delta) {
	float movementSpeed = delta / baseSpeed;
	
	//Movement
	if (glfwGetKey(windowC, GLFW_KEY_W) == GLFW_PRESS) {
		eye += forward * movementSpeed;
		center += forward * movementSpeed;
	}
	if (glfwGetKey(windowC, GLFW_KEY_S) == GLFW_PRESS) {
		eye -= forward * movementSpeed;
		center -= forward * movementSpeed;
	}
	if (glfwGetKey(windowC, GLFW_KEY_A) == GLFW_PRESS) {
		eye -= right * movementSpeed;
		center -= right * movementSpeed;
	}
	if (glfwGetKey(windowC, GLFW_KEY_D) == GLFW_PRESS) {
		eye += right * movementSpeed;
		center += right * movementSpeed;
	}

	//Vertical
	if (glfwGetKey(windowC, GLFW_KEY_SPACE) == GLFW_PRESS) {
		eye += up * movementSpeed;
		center += up * movementSpeed;
	}
	if (glfwGetKey(windowC, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		eye -= up * movementSpeed;
		center -= up * movementSpeed;
	}

	//Rotation
	if (glfwGetKey(windowC, GLFW_KEY_LEFT) == GLFW_PRESS) {
		center = rotateAroundAxis(center - eye, up, glm::radians(40.0f * movementSpeed)) + eye;
		forward = glm::normalize(center - eye);
		right = glm::normalize(glm::cross(forward, up));
	}
	if (glfwGetKey(windowC, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		center = rotateAroundAxis(center - eye, up, glm::radians(-40.0f * movementSpeed)) + eye;
		forward = glm::normalize(center - eye);
		right = glm::normalize(glm::cross(forward, up));
	}
	if (glfwGetKey(windowC, GLFW_KEY_UP) == GLFW_PRESS) {
		center = rotateAroundAxis(center - eye, right, glm::radians(40.0f * movementSpeed)) + eye;
		forward = glm::normalize(center - eye);
	}
	if (glfwGetKey(windowC, GLFW_KEY_DOWN) == GLFW_PRESS) {
		center = rotateAroundAxis(center - eye, right, glm::radians(-40.0f * movementSpeed)) + eye;
		forward = glm::normalize(center - eye);
	}
}

glm::mat4 view;
glm::mat4 projection;

void Camera::updateUniforms() {
	view = glm::lookAt(eye, center, up);

	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.f), 1280.f / 720.f, 0.01f, 100.0f);
}

void Camera::updateGroundUniforms() {
	glLinkProgram(Shader::groundShaderProgram);
	glUseProgram(Shader::groundShaderProgram);
	
	GLint viewGroundLocation = glGetUniformLocation(Shader::groundShaderProgram, "view");
	GLint projectionGroundLocation = glGetUniformLocation(Shader::groundShaderProgram, "projection");
	
	glUniformMatrix4fv(viewGroundLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionGroundLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::updateGrassUniforms() {
	glLinkProgram(Shader::grassShaderProgram);
	glUseProgram(Shader::grassShaderProgram);
	
	GLint viewGrassLocation = glGetUniformLocation(Shader::grassShaderProgram, "grassView");
	GLint projectionGrassLocation = glGetUniformLocation(Shader::grassShaderProgram, "grassProj");
	
	glUniformMatrix4fv(viewGrassLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionGrassLocation, 1, GL_FALSE, glm::value_ptr(projection));
}