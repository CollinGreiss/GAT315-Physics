#include "world.h"

#include <stdlib.h>
#include <assert.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody() {

	Body* body = malloc( sizeof( Body ) );
	assert( body );

	body->next = bodies;
	body->prev = NULL;

	bodyCount++;
	if ( bodies ) bodies->prev = body;
	bodies = body;

	return body;

}

void DestoryBody( Body* body ) {

	assert( body );

	if ( body->prev ) body->prev->next = body->next;
	if ( body->next ) body->next->prev = body->prev;

	if ( body == bodies ) bodies = bodies->next;

	bodies--;
	free( body );

}

