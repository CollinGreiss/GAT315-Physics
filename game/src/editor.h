#pragma once
#include "raylib.h"

typedef struct ncEditorData {

    bool EditorWindowActive;
    
    int WindowType;
    int WindowTypeEditMode;
   
    Vector2 WindowSize;
    Vector2 anchor;

	float TimeStep;
    bool Simulating;
    bool Reset;

    //World
    float Gravitation;

    //Body
    float MassMin;
    float MassMax;
    float GravityScale;
    
    bool BodyTypeEditMode;
    int BodyType;
    
    float Damping;
    float Restitution;
    
    bool RandomColor;
    Color BodyColor;

    //Spring
    float Stiffness;

} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor( Vector2 position );
void DrawEditor( Vector2 position );

struct ncBody* GetBodyIntersect( struct ncBody* bodies, Vector2 position );
void DrawLineBodyToPosition( struct ncBody* body, Vector2 position );