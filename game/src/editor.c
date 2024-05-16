#include "editor.h"

#include "body.h"

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

	ncEditorData.anchor = ( Vector2 ) { 10, 80 };
	ncEditorData.BodyWindowActive = false;
	ncEditorData.MassMin = 0.5f;
	ncEditorData.MassMax = 3.0f;
	ncEditorData.Gravity = 1.0f;
	ncEditorData.BodyTypeEditMode = false;
	ncEditorData.BodyType = 2;
	ncEditorData.Damping = 0.0f;
	ncEditorData.Gravitation = 0.0f;

	editorRect = ( Rectangle ) { ncEditorData.anchor.x + 0, ncEditorData.anchor.y + 0, 280, 280 };

}

void UpdateEditor( Vector2 position ) {

	if ( IsKeyPressed( KEY_TAB ) ) ncEditorData.BodyWindowActive = !ncEditorData.BodyWindowActive;

	ncEditorIntersect = ncEditorData.BodyWindowActive && CheckCollisionPointRec( position, editorRect );

}

void DrawEditor( Vector2 position ) {

	if ( ncEditorData.BodyTypeEditMode ) GuiLock();

	if ( ncEditorData.BodyWindowActive ) {

		ncEditorData.BodyWindowActive = !GuiWindowBox( ( Rectangle ) { ncEditorData.anchor.x, ncEditorData.anchor.y, 280, 280 }, "Body" );

		GuiLabel( ( Rectangle ) { 136 + ncEditorData.anchor.x, 40 + ncEditorData.anchor.y, 100, 24 }, TextFormat( "Min Mass: %.2f", ncEditorData.MassMin ) );
		GuiSlider( ( Rectangle ) { 8 + ncEditorData.anchor.x, 40 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.MassMin, 0.01f, ncEditorData.MassMax );

		GuiLabel( ( Rectangle ) { 136 + ncEditorData.anchor.x, 80 + ncEditorData.anchor.y, 110, 24 }, TextFormat( "Max Mass: %.2f", ncEditorData.MassMax ) );
		GuiSlider( ( Rectangle ) { 8 + ncEditorData.anchor.x, 80 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.MassMax, ncEditorData.MassMin, 100 );

		GuiLabel( ( Rectangle ) { 136 + ncEditorData.anchor.x, 160 + ncEditorData.anchor.y, 140, 24 }, TextFormat( "Gravity Scale: %.2f", ncEditorData.Gravity ) );
		GuiSlider( ( Rectangle ) { 8 + ncEditorData.anchor.x, 160 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.Gravity, 0, 100 );

		GuiLabel( ( Rectangle ) { 136 + ncEditorData.anchor.x, 200 + ncEditorData.anchor.y, 140, 24 }, TextFormat( "Damping: %.2f", ncEditorData.Damping ) );
		GuiSlider( ( Rectangle ) { 8 + ncEditorData.anchor.x, 200 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.Damping, 0, 100 );

		GuiLabel( ( Rectangle ) { 136 + ncEditorData.anchor.x, 240 + ncEditorData.anchor.y, 160, 24 }, TextFormat( "Gravitation Scale: %.2f", ncEditorData.Gravitation ) );
		GuiSlider( ( Rectangle ) { 8 + ncEditorData.anchor.x, 240 + ncEditorData.anchor.y, 120, 24 }, NULL, NULL, & ncEditorData.Gravitation, 0, 100 );

		GuiLabel( ( Rectangle ) { 136 + ncEditorData.anchor.x, 120 + ncEditorData.anchor.y, 120, 24 }, "Body Type" );
		if ( GuiDropdownBox( ( Rectangle ) { 8 + ncEditorData.anchor.x, 120 + ncEditorData.anchor.y, 120, 24 }, "Static;Kinematic;Dynamic", & ncEditorData.BodyType, ncEditorData.BodyTypeEditMode ) )
			ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;

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