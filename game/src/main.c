#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include <world.h>

#include <stdlib.h>
#include <assert.h>

int main( void ) {

	InitWindow( 1280, 720, "Physics Engine" );
	SetTargetFPS( 60 );

	while ( !WindowShouldClose() ) {

		//Update
		float dt = GetFrameTime();
		float fps = (float) GetFPS();

		Vector2 position = GetMousePosition();

		if ( IsMouseButtonDown( 0 ) ) {

			Body* body = CreateBody();
			body->position = position;
			body->velocity = CreateVector2( GetRandomFloatValue( -5, 5 ), GetRandomFloatValue( -5, 5 ) );


		}

		//Render
		BeginDrawing();
		ClearBackground( BLACK );

		Body* body = bodies;
		while ( body ) {
			
			body->position = Vector2Add( body->position, body->velocity );
			DrawCircle( (int) body->position.x, body->position.y, 10, RED );

			body = body->next;

		}

		DrawText( TextFormat( "FPS: %.2f (%.2fms)", fps, 1000 / fps ), 10, 10, 20, LIME );
		DrawText( TextFormat( "FRAME: %.4f", dt ), 10, 30, 20, LIME );

		DrawCircleLines( (int) position.x, (int) position.y, 10, WHITE );

		EndDrawing();

	}

	CloseWindow();

	free( bodies );

	return 0;

}