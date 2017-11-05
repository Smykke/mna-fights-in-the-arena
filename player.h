#ifndef PLAYER_H
#define PLAYER_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>
// Dimensions
#define radiusWheel 30

class Player {
	GLfloat gX;
	GLfloat gY;
	GLfloat gThetaWheel;
	GLfloat direction;
	bool switchLeg;
	GLfloat sWidth;
	GLfloat sHeight;
	GLfloat legHeight;
	GLfloat legWidth;
	GLint color[3];
	GLint colorBorder[3];


private:
	void twoSeconds(int jumpTime);
	void DesenhaRect(	GLint height, GLint width,
				GLint forColor[], GLint borColor[]);
	void DrawGun( GLfloat gX, GLfloat gY, GLfloat gTheta1);
	void DrawLegs(GLfloat gX, GLfloat gY);
	void DrawHeadShoulder(GLfloat radiusX, GLfloat radiusY);
	void DrawPlayer(GLfloat gX, GLfloat gY,	GLfloat shoulderWidth, GLfloat shoulderHeight,
		 		GLfloat gThetaG, GLfloat gThetaP, GLfloat headRadius);

public:

	GLfloat hRadius;
	GLfloat gThetaGun;
	GLfloat gThetaPl;

	Player() {
		gX = 0;
		gY = 0;
		gThetaGun = 0;
		gThetaPl = 0;
		gThetaWheel = 0;
		direction = -1;
		switchLeg = true;
		hRadius = 80;
		sWidth = hRadius*2;
		sHeight = hRadius/2;
		legHeight = hRadius*2;
		legWidth = hRadius/2;
		color[0] = 0;
		color[1] = 1;
		color[2] = 0;
		colorBorder[0] = 1;
		colorBorder[1] = 1;
		colorBorder[2] = 1;
	};

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

	void jump(int jumpTime);

	// void RodaBraco1(GLfloat gTheta1) {
	// 	this->gTheta1 += gTheta1;
	// };
	// void RodaBraco2(GLfloat gTheta2) {
	// 	this->gTheta2 += gTheta2;
	// };
	// void RodaBraco3(GLfloat gTheta3) {
	// 	this->gTheta3 += gTheta3;
	// };
};

#endif
