#pragma once
#include "raylib.h"

typedef struct ncEditorData {

    bool BodyWindowActive;
    float MassMin;
    float MassMax;
    float Gravity;
    bool BodyTypeEditMode;
    int BodyType;
    float Damping;
    float Gravitation;

	Vector2 anchor;

} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor( Vector2 position );
void DrawEditor( Vector2 position );

struct ncBody* GetBodyIntersect( struct ncBody* bodies, Vector2 position );
void DrawLineBodyToPosition( struct ncBody* body, Vector2 position );