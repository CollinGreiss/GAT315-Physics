#pragma once

#include "Raylib.h"

typedef struct Body {

	Vector2 position;
	Vector2 velocity;
	Vector2 force;

	struct Body* next;
	struct Body* prev;

} Body;