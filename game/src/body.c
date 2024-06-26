#include "body.h"

#include "intergrator.h"
#include "world.h"

void ApplyForce( ncBody* body, Vector2 force, ncForceMode forceMode ) {

	if ( body->type != BT_DYNAMIC ) return;

	switch ( forceMode ) {

	case FM_FORCE:
		body->force = Vector2Add( body->force, force );
		break;

	case FM_IMPULSE:
		body->velocity = Vector2Add( body->velocity, Vector2Scale( force, body->inverseMass ) );
		break;

	case FM_VELOCITY:
		body->velocity = force;
		break;

	}

}

void ClearForce( ncBody* body ) {

	body->force = Vector2Zero();

}

void Step( ncBody* body, float timestep ) {

	body->force = Vector2Add( body->force, Vector2Scale( Vector2Scale( ncGravity, body->gravityScale ), body->mass ) );
	body->acceleration = Vector2Scale( body->force, body->inverseMass );

	float damping = 1 / ( 1 + ( body->damping * timestep ) );
	body->velocity = Vector2Scale( body->velocity, damping );

	SemiImplicitEuler( body, timestep );
	ClearForce( body );


}
