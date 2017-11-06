#ifndef CIRCLE_H
#define CIRCLE_H
#include <GL/glut.h>
#include <math.h>

typedef struct circle {
  GLfloat cx, cy, radius;
  GLfloat r, g, b;
} Circle;

float getPlayerDistance(float pX, float pY, Circle obj);

#endif
