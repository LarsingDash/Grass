#include "Camera.h"
#include "Shader.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Camera::updateCamera() {
	//View
	glm::mat4 view = glm::mat4(1.0f);

	glm::vec3 eye = glm::vec3(2.0f, 0.0f, 3.0f);
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	view = glm::lookAt(eye, center, up);

	GLint viewLocation = glGetUniformLocation(Shader::shaderProgram, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	//Projection
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.f), 1280.f / 720.f, 0.01f, 100.0f);

	GLint projectionLocation = glGetUniformLocation(Shader::shaderProgram, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}
