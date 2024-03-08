#ifndef GRASS_INPUT_H
#define GRASS_INPUT_H

#include <map>
#include "Main.h"

namespace Input {
	typedef void(* inputFunc)();
	extern std::map<int, std::pair<bool, inputFunc>> inputList;

	void assignInput(int key, inputFunc input);
	void updateInputs(GLFWwindow* window);
}

#endif //GRASS_INPUT_H
