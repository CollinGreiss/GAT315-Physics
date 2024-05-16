#pragma once

#include "raylib.h"
#include "raymath.h"

typedef enum {

	BT_STATIC,
	BT_KINEMATIC,
	BT_DYNAMIC

} ncBodyType;

typedef enum {

	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY

} ncForceMode;

typedef struct ncBody {

	ncBodyType type;

	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	float mass;
	float inverseMass;
	float gravityScale;
	float damping;

	float restitution;

	Color color;

	struct ncBody* next;
	struct ncBody* prev;

} ncBody;

void ApplyForce( ncBody* body, Vector2 force, ncForceMode forceMode );

void ClearForce( ncBody* body );

void Step( ncBody* body, float timestep );