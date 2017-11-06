#ifndef SHOT_H
#define SHOT_H
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "circle.h"

class Shot {
  // GLint radius;
  GLfloat sX;
  GLfloat sY;
  GLfloat dX;
  GLfloat dY;
  GLfloat sTheta;
  // GLfloat speed;

public:
  Shot() {
    sX = 0;
    sY = 0;
    dX = 0;
    dY = 0;
    sTheta = 0;
  }

  Shot(GLfloat gX, GLfloat gY, GLfloat dX, GLfloat dY, GLfloat gTheta) {
    this->sX = gX;
    this->sY = gY;
    this->sTheta = gTheta;
  }

  ~Shot() {
    printf("Shot is out\n");
  }

  Shot Shoot(GLfloat gX, GLfloat gY, GLfloat gTheta) {
    GLfloat deltaX = 
    return Shot(gX, gY, gTheta);
  }

  void MoveShot() {
    this->sX += 1;
    this->sY += 1;
  }
}

#endif
