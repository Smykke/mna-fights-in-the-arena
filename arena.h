#ifndef ARENA_H
#define ARENA_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <vector>


typedef struct circle {
  GLfloat cx, cy, radius;
  GLfloat r, g, b;
} Circle;

void drawCircle (GLfloat x, GLfloat y, GLfloat rd, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f (r, g, b);

  glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * M_PI; i += M_PI/18)
      glVertex3f( cos(i) * rd + x, sin(i) * rd + y, 0.0 );
  glEnd();
}
#endif
