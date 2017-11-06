#ifndef PLAYER_H
#define PLAYER_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "circle.h"


class Player {
	GLfloat direction;
	GLfloat sWidth;
	GLfloat sHeight;
	GLfloat legHeight;
	GLfloat legWidth;
	GLint color[3];
	GLint colorBorder[3];
	GLfloat originalRadius;
	GLfloat maxRadius;


private:
	void DesenhaRect(	GLint height, GLint width,
				GLint forColor[], GLint borColor[]);
	void DrawGun( GLfloat gX, GLfloat gY, GLfloat gTheta1);
	void DrawLegs(GLfloat gX, GLfloat gY);
	void DrawHeadShoulder(GLfloat radiusX, GLfloat radiusY);
	void DrawPlayer(GLfloat gX, GLfloat gY,	GLfloat shoulderWidth, GLfloat shoulderHeight,
		 		GLfloat gThetaG, GLfloat gThetaP, GLfloat headRadius);

public:
	GLfloat gX;
	GLfloat gY;
	GLfloat hRadius;
	GLfloat gThetaGun;
	GLfloat gThetaPl;
	bool jumping;
	bool inside;
	bool switchLeg;

	Player() { // verificar: necessario?
		gX = 0;
		gY = 0;
		gThetaGun = 0;
		gThetaPl = 0;
	};

	Player(GLfloat centerX, GLfloat centerY, GLfloat radius) {
		gX = centerX;
		gY = centerY;
		gThetaGun = 0;
		gThetaPl = 0;
		direction = -1;
		switchLeg = true;
		jumping = false;
		inside = false;
		hRadius = radius;
		sWidth = hRadius*2;
		sHeight = hRadius/2;
		legHeight = hRadius*1.4;
		legWidth = hRadius/2;
		color[0] = 0;
		color[1] = 1;
		color[2] = 0;
		colorBorder[0] = 0;
		colorBorder[1] = 0;
		colorBorder[2] = 0;
		maxRadius = this->hRadius*1.5;
		originalRadius = this->hRadius;
		// printf("maxR: %.2f, oriR: %.2f\n", maxRadius, originalRadius);
	};

	float twoSeconds(int jumpTime);

	bool jump(int jumpTime, std::vector<Circle> obstacles);

	void Draw() {
			DrawPlayer(gX, gY, sWidth, sHeight, gThetaGun, gThetaPl, hRadius);
	};

	void rotateGun(GLfloat dTheta) {
		this->gThetaGun += dTheta;
	};

	void RotatePlayer(GLfloat dTheta) {
		this->gThetaPl += dTheta;
	};

	void MovePlayerX(GLfloat gX) {
		this->gX += gX;
	};

	void MovePlayerY(GLfloat gY) {
		this->gY += gY;
	};

	void ResizePlayer(GLfloat newRadius) {
		this->hRadius = newRadius;
		this->sWidth = hRadius*2;
		this->sHeight = hRadius/2;
		this->legHeight = hRadius*2;
		this->legWidth = hRadius/2;
	}
};

#endif
