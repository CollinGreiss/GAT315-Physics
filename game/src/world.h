#pragma once

#include "body.h"

extern Body* bodies;
extern int bodyCount;

Body* CreateBody();
void DestoryBody( Body* body );