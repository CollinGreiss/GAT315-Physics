#include "world.h"
#include "raymath.h"

#include "mathf.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;

Vector2 ncGravity;

ncBody* CreateBody( Vector2 position, float mass, ncBodyType type ) {

	ncBody* body = malloc( sizeof( ncBody ) );
	assert( body );

	memset( body, 0, sizeof( ncBody ) );

	body->position = position;
	body->mass = mass;
	body->inverseMass = (type == BT_STATIC) ? 0 : 1 / mass;
	body->type = type;
	body->gravityScale = 1;
	body->color = ( Color ) { GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255};

	return body;

}

void AddBody( ncBody* body ) {

	body->next = ncBodies;
	body->prev = NULL;

	ncBodyCount++;
	if ( ncBodies ) ncBodies->prev = body;
	ncBodies = body;

}

void DestoryBody( ncBody* body ) {

	assert( body );

	if ( body->prev ) body->prev->next = body->next;
	if ( body->next ) body->next->prev = body->prev;

	if ( body == ncBodies ) ncBodies = ncBodies->next;

	ncBodyCount--;
	free( body );

}

void DestoryAllBodies() {

	ncBody* prevBody = ncBodies;
	while ( ncBodies ) {

		prevBody = ncBodies;
		ncBodies = ncBodies->next;
		DestoryBody( prevBody );

	}

}
