#include "spring.h"

#include "body.h"
#include <assert.h>
#include <stdlib.h>


ncSpring_t* ncSprings;
int ncSpringCount;

ncSpring_t* CreateSpring( ncBody* body1, ncBody* body2, float restLength, float k ) {

	ncSpring_t* spring = malloc( sizeof( ncSpring_t ) );
	assert( spring );

	memset( spring, 0, sizeof( ncSpring_t ) );

	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	return spring;

}

void AddSpring( ncSpring_t* spring ) {

	spring->next = ncSprings;
	spring->prev = NULL;

	ncSpringCount++;
	if ( ncSprings ) ncSprings->prev = spring;
	ncSprings = spring;

}

void DestroySpring( ncSpring_t* spring ) {

	assert( spring );

	if ( spring->prev ) spring->prev->next = spring->next;
	if ( spring->next ) spring->next->prev = spring->prev;

	if ( spring == ncSprings ) ncSprings = ncSprings->next;

	ncSpringCount--;
	free( spring );

}

void DestoryAllSprings() {

	ncBody* prevSpring = ncSprings;
	while ( ncSprings ) {

		prevSpring = ncSprings;
		ncSprings = ncSprings->next;
		DestroySpring( prevSpring );

	}

}

void ApplySpringForce(  ) {

	for ( ncSpring_t* spring = ncSprings; spring; spring = spring->next ) {

		Vector2 direction = Vector2Subtract( spring->body1->position, spring->body2->position );

		if ( direction.x == 0 && direction.y == 0 ) continue;

		float length = Vector2Length( direction );
		float x = length - spring->restLength;
		float force = -spring->k * x; 

		Vector2 ndirection = Vector2Normalize( direction );

		ApplyForce( spring->body1, Vector2Scale(ndirection, force ), FM_FORCE);
		ApplyForce( spring->body2, Vector2Scale( ndirection, -force ), FM_FORCE );
	}

}
