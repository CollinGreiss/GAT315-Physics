#pragma once

typedef struct ncSpring {

	struct ncBody* body1;
	struct ncBody* body2;
	float restLength;
	float k;

	struct ncSpring* next;
	struct ncSpring* prev;

} ncSpring_t;

extern ncSpring_t* ncSprings;
extern int ncSpringCount;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k);
void AddSpring( ncSpring_t* spring );
void DestroySpring( ncSpring_t* spring );
void DestoryAllSprings();

void ApplySpringForce( );