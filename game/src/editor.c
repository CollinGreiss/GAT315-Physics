#include "editor.h"

#include "body.h"
#include "world.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include "render.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
Rectangle editorRect;

ncEditorData_t ncEditorData;
Texture2D cursorTexture;

bool EditorBoxActive = true;

void InitEditor() {

	GuiLoadStyle( "raygui/styles/cyber/style_cyber.rgs" );

	Image image = LoadImage( "resources/reticle.png" );
	cursorTexture = LoadTextureFromImage( image );
	UnloadImage( image );
	HideCursor();

	ncEditorData.TimeStep = 60.0f;
	ncEditorData.Simulating = true;
	ncEditorData.Reset = false;

	ncEditorData.anchor = ( Vector2 ) { 10, 80 };
	ncEditorData.EditorWindowActive = false;
	ncEditorData.WindowType = 0;
	ncEditorData.WindowTypeEditMode = false;

	ncEditorData.Gravitation = 0.0f;

	ncEditorData.MassMin = 0.5f;
	ncEditorData.MassMax = 3.0f;
	ncEditorData.GravityScale = 1.0f;
	ncEditorData.BodyTypeEditMode = false;
	ncEditorData.BodyType = 2;
	ncEditorData.Damping = 0.0f;
	ncEditorData.Restitution = 0.0f;
	ncEditorData.RandomColor = true;
	ncEditorData.BodyColor = WHITE;

	ncEditorData.Stiffness = 20.0f;

	editorRect = ( Rectangle ) { ncEditorData.anchor.x + 0, ncEditorData.anchor.y + 0, (int) ncEditorData.WindowSize.x, (int) ncEditorData.WindowSize.y };

}

void UpdateEditor( Vector2 position ) {

	if ( IsKeyPressed( KEY_TAB ) ) ncEditorData.EditorWindowActive = !ncEditorData.EditorWindowActive;

	ncEditorIntersect = ncEditorData.EditorWindowActive && CheckCollisionPointRec( position, editorRect );

}

void DrawWorldWindow() {

	ncEditorData.WindowSize.x = 320;
	ncEditorData.WindowSize.y = 240;

	GuiGroupBox( ( Rectangle ) { 20 + ncEditorData.anchor.x, 80 + ncEditorData.anchor.y, ncEditorData.WindowSize.x - 40, ncEditorData.WindowSize.y - 100 }, "World" );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 100 + ncEditorData.anchor.y, 160, 24 }, TextFormat( "Gravitation Scale: %.2f", ncEditorData.Gravitation ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 100 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.Gravitation, 0, 100 );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 140 + ncEditorData.anchor.y, 110, 24 }, TextFormat( "Gravity X: %.2f", ncGravity.x ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 140 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncGravity.x, -5, 5 );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 180 + ncEditorData.anchor.y, 110, 24 }, TextFormat( "Gravity Y: %.2f", ncGravity.y ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 180 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncGravity.y, -5, 5 );

}

void DrawSpringWindow() {

	ncEditorData.WindowSize.x = 280;
	ncEditorData.WindowSize.y = 160;

	GuiGroupBox( ( Rectangle ) { 20 + ncEditorData.anchor.x, 80 + ncEditorData.anchor.y, ncEditorData.WindowSize.x - 40, ncEditorData.WindowSize.y - 100 }, "Spring" );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 100 + ncEditorData.anchor.y, 160, 24 }, TextFormat( "Stiffness: %.2f", ncEditorData.Stiffness ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 100 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.Stiffness, 0, 100 );

}

void DrawBodyWindow() {

	ncEditorData.WindowSize.x = 300;
	ncEditorData.WindowSize.y = 400;
	if ( !ncEditorData.RandomColor ) ncEditorData.WindowSize.y += 80;

	GuiGroupBox( ( Rectangle ) { 20 + ncEditorData.anchor.x, 80 + ncEditorData.anchor.y, ncEditorData.WindowSize.x - 40, ncEditorData.WindowSize.y - 100 }, "Body" );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 100 + ncEditorData.anchor.y, 100, 24 }, TextFormat( "Min Mass: %.2f", ncEditorData.MassMin ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 100 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.MassMin, 0.01f, ncEditorData.MassMax );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 140 + ncEditorData.anchor.y, 110, 24 }, TextFormat( "Max Mass: %.2f", ncEditorData.MassMax ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 140 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.MassMax, ncEditorData.MassMin, 10 );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 220 + ncEditorData.anchor.y, 140, 24 }, TextFormat( "Gravity Scale: %.2f", ncEditorData.GravityScale ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 220 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.GravityScale, 0, 100 );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 260 + ncEditorData.anchor.y, 140, 24 }, TextFormat( "Damping: %.2f", ncEditorData.Damping ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 260 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.Damping, 0, 100 );

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 300 + ncEditorData.anchor.y, 140, 24 }, TextFormat( "Restitution: %.2f", ncEditorData.Restitution ) );
	GuiSlider( ( Rectangle ) { 28 + ncEditorData.anchor.x, 300 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.Restitution, 0, 1 );

	if ( !ncEditorData.RandomColor ) {

		GuiLabel( ( Rectangle ) { 28 + ncEditorData.anchor.x, 330 + ncEditorData.anchor.y, 140, 24 }, "Body Color" );
		GuiColorPicker( ( Rectangle ) { 28 + ncEditorData.anchor.x, 350 + ncEditorData.anchor.y, 96, 96 }, NULL, & ncEditorData.BodyColor );

		GuiLabel( ( Rectangle ) { 176 + ncEditorData.anchor.x, 360 + ncEditorData.anchor.y, 140, 24 }, "Random Color" );
		GuiCheckBox( ( Rectangle ) { 200 + ncEditorData.anchor.x, 390 + ncEditorData.anchor.y, 24, 24 }, NULL, & ncEditorData.RandomColor );

	} else {

		GuiLabel( ( Rectangle ) { 60 + ncEditorData.anchor.x, 340 + ncEditorData.anchor.y, 140, 24 }, "Random Color" );
		GuiCheckBox( ( Rectangle ) { 28 + ncEditorData.anchor.x, 340 + ncEditorData.anchor.y, 24, 24 }, NULL, & ncEditorData.RandomColor );
	}

	GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 180 + ncEditorData.anchor.y, 120, 24 }, "Body Type" );
	if ( GuiDropdownBox( ( Rectangle ) { 28 + ncEditorData.anchor.x, 180 + ncEditorData.anchor.y, 120, 24 }, "Static;Kinematic;Dynamic", & ncEditorData.BodyType, ncEditorData.BodyTypeEditMode ) )
		ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;

}

void DrawEditor( Vector2 position ) {

	editorRect = ( Rectangle ) { ncEditorData.anchor.x + 0, ncEditorData.anchor.y + 0, (int) ncEditorData.WindowSize.x, (int) ncEditorData.WindowSize.y };

	if ( ncEditorData.BodyTypeEditMode ) GuiLock();

	if ( ncEditorData.EditorWindowActive ) {

		ncEditorData.EditorWindowActive = !GuiWindowBox( ( Rectangle ) { ncEditorData.anchor.x, ncEditorData.anchor.y, (int) ncEditorData.WindowSize.x, (int) ncEditorData.WindowSize.y }, "Editor" );

		switch ( ncEditorData.WindowType ) {

		case 0:
			DrawWorldWindow();
			break;

		case 1:
			DrawBodyWindow();
			break;

		case 2:
			DrawSpringWindow();
			break;

		}

		GuiToggle( ( Rectangle ) {

			30 + ncEditorData.anchor.x,
				ncEditorData.anchor.y + ncEditorData.WindowSize.y,
				ncEditorData.WindowSize.x * 0.5 - 40,
				30

		}, "Simulate", & ncEditorData.Simulating );

		ncEditorData.Reset = GuiButton( ( Rectangle ) {

			10 + ncEditorData.anchor.x + ncEditorData.WindowSize.x * 0.5,
				ncEditorData.anchor.y + ncEditorData.WindowSize.y,
				ncEditorData.WindowSize.x * 0.5 - 40,
				30

		}, "Reset" );

		GuiLabel( ( Rectangle ) { 
			
			10 + ncEditorData.anchor.x + ncEditorData.WindowSize.x * 0.5,
				40 + ncEditorData.anchor.y + ncEditorData.WindowSize.y,
				ncEditorData.WindowSize.x * 0.5 - 40,
				20
		
		}, TextFormat( "Timestep: %.2f", ncEditorData.TimeStep ) );

		GuiSlider( ( Rectangle ) { 
			
			30 + ncEditorData.anchor.x,
				40 + ncEditorData.anchor.y + ncEditorData.WindowSize.y,
				ncEditorData.WindowSize.x * 0.5 - 40,
				20
		
		}, NULL, NULL, & ncEditorData.TimeStep, 0, 165 );

		ncEditorData.WindowSize.y += 80;

		GuiLabel( ( Rectangle ) { 156 + ncEditorData.anchor.x, 40 + ncEditorData.anchor.y, 120, 24 }, "Window" );
		if ( GuiDropdownBox( ( Rectangle ) { 28 + ncEditorData.anchor.x, 40 + ncEditorData.anchor.y, 120, 24 }, "World;Body;Spring", & ncEditorData.WindowType, ncEditorData.WindowTypeEditMode ) )
			ncEditorData.WindowTypeEditMode = !ncEditorData.WindowTypeEditMode;

	}

	DrawTexture( cursorTexture, (int) position.x - cursorTexture.width / 2, (int) position.y - cursorTexture.height / 2, WHITE );

	GuiUnlock();

}

ncBody* GetBodyIntersect( ncBody* bodies, Vector2 position ) {

	for ( ncBody* body = bodies; body; body = body->next ) {

		Vector2 screen = ConvertWorldToScreen( body->position );
		if ( CheckCollisionPointCircle( position, screen, ConvertWorldToPixel( body->mass * 0.5f ) ) ) {

			return body;

		}

	}

	return NULL;

}

void DrawLineBodyToPosition( ncBody* body, Vector2 position ) {

	Vector2 screen = ConvertWorldToScreen( body->position );
	DrawLine( (int) screen.x, (int) screen.y, (int) position.x - cursorTexture.width / 2, (int) position.y - cursorTexture.height / 2, YELLOW );

}