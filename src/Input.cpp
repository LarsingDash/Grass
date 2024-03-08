#include "Input.h"

namespace Input {
	std::map<int, std::pair<bool, inputFunc>> inputList;
}

void Input::assignInput(int key, inputFunc input) {
	inputList[key] = std::pair<bool, inputFunc>(false, input);
}

void Input::updateInputs(GLFWwindow* window) {
	for (auto& input: inputList) {
		if (glfwGetKey(window, input.first) == GLFW_PRESS) {
			if (!input.second.first) {
				input.second.first = true;
				input.second.second();
			}
		} else input.second.first = false;
	}
}