#include "circle.h"
float getPlayerDistance(float pX, float pY, Circle obj) {
  return sqrt(pow(pX - obj.cx, 2) + pow(pY - obj.cy, 2));
}
