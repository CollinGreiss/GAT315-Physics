#include "world.h"
#include "body.h"

#include "editor.h"
#include "render.h"

#include "intergrator.h"
#include "mathf.h"

#include "force.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"

#include "raylib.h"
#include "raymath.h"
#include "../../raygui/src/raygui.h"

#include <stdlib.h>
#include <assert.h>



int main( void ) {

	ncBody* selectedBody = NULL;
	ncBody* connectedBody = NULL;
	ncContact_t* contacts = NULL;

	ncGravity = ( Vector2 ) { 0, -1 };
	float timeAccumulator = 0;

	InitWindow( (int) ncScreenSize.x, (int) ncScreenSize.y, "Physics Engine" );
	InitEditor();
	SetTargetFPS( 60 );

	while ( !WindowShouldClose() ) {

		//Update

		float dt = GetFrameTime();
		float fps = (float) GetFPS();
		float timestep = 1.0 / ncEditorData.TimeStep;

		Vector2 position = GetMousePosition();

		UpdateEditor( position );

		selectedBody = GetBodyIntersect( ncBodies, position );
		if ( selectedBody ) {

			Vector2 screen = ConvertWorldToScreen( selectedBody->position );
			DrawCircleLines( screen.x, screen.y, ConvertWorldToPixel( selectedBody->mass * 0.5f ), YELLOW );

		}

		if ( !ncEditorIntersect
			&& ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT )
			|| ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) && IsKeyDown( KEY_LEFT_CONTROL ) ) ) ) {

			ncBody* body = CreateBody( ConvertScreenToWorld( position ), GetRandomFloatValue( ncEditorData.MassMin, ncEditorData.MassMax ), ncEditorData.BodyType );
			assert( body );

			AddBody( body );

		}

		if ( IsMouseButtonPressed( MOUSE_BUTTON_RIGHT ) && selectedBody ) connectedBody = selectedBody;
		if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) && connectedBody ) DrawLineBodyToPosition( connectedBody, position );
		if ( IsMouseButtonReleased( MOUSE_BUTTON_RIGHT ) && connectedBody ) {

			if ( selectedBody && selectedBody != connectedBody && ( selectedBody->type != BT_STATIC || connectedBody->type != BT_STATIC ) ) {

				ncSpring_t* spring = CreateSpring( connectedBody, selectedBody, Vector2Distance( connectedBody->position, selectedBody->position ), 20 );
				AddSpring( spring );

			}

		}
		
		timeAccumulator += dt;
		while ( timeAccumulator >= timestep ) {

			timeAccumulator -= timestep;
			if ( !ncEditorData.Simulating ) continue;
			if ( ncEditorData.Reset ) {

				DestroyAllContacts( &contacts );
				DestoryAllSprings();
				DestoryAllBodies();
				ncEditorData.Reset = false;
				continue;

			}

			ApplyGravitation( ncEditorData.Gravitation );
			ApplySpringForce( ncSprings );

			for ( ncBody* body = ncBodies; body; body = body->next ) {

				Step( body, dt );

			}

			DestroyAllContacts( &contacts );
			CreateContacts( ncBodies, &contacts );
			SeparateContacts( contacts );
			ResolveContacts( contacts );

		}

		//Render
		BeginDrawing();
		ClearBackground( BLACK );

		for ( ncSpring_t* spring = ncSprings; spring; spring = spring->next ) {

			Vector2 screen1 = ConvertWorldToScreen( spring->body1->position );
			Vector2 screen2 = ConvertWorldToScreen( spring->body2->position );
			DrawLine( (int) screen1.x, (int) screen1.y, (int) screen2.x, (int) screen2.y, YELLOW );

		}

		for ( ncBody* body = ncBodies; body; body = body->next ) {

			Vector2 bodyPos = ConvertWorldToScreen( body->position );

			DrawCircle( (int) bodyPos.x, bodyPos.y, ConvertWorldToPixel( body->mass * 0.5f ), body->color );

		}

		for ( ncContact_t* contact = contacts; contact; contact = contact->next ) {

			Vector2 screen = ConvertWorldToScreen( contact->body1->position );
			DrawCircleLines( (int) screen.x, (int) screen.y, ConvertWorldToPixel( contact->body1->mass * 0.5f ), RED );

		}

		DrawText( TextFormat( "FPS: %.2f (%.2fms)", fps, 1000 / fps ), 10, 10, 20, LIME );
		DrawText( TextFormat( "FRAME: %.4f", dt ), 10, 30, 20, LIME );
		DrawText( TextFormat( "Bodies: %i", ncBodyCount ), 10, 50, 20, LIME );

		DrawCircleLines( (int) position.x, (int) position.y, 10, WHITE );

		DrawEditor( position );

		EndDrawing();

	}

	DestroyAllContacts( &contacts );
	DestoryAllSprings();
	DestoryAllBodies();

	CloseWindow();

	return 0;

}