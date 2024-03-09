#ifndef GRASS_GRASS_H
#define GRASS_GRASS_H

#include "../Main.h"

namespace Grass {
	void grassInit();
	void spawn();
	void windData();
	void draw();
	void update(float delta);
	void assignInputs();
	void grassDestroy();
	
	static unsigned int grassVBO, grassVAO;
}

#endif //GRASS_GRASS_H
