#ifndef ROBO_H
#define ROBO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>
// Dimensions
#define radiusWheel 30

class Robo {
	GLfloat gX;
	GLfloat gY;
	GLfloat gTheta2;
	GLfloat gTheta3;
	GLfloat gThetaWheel;
	GLfloat direction;
	bool switchLeg;
	GLfloat hRadius;
	GLfloat sWidth;
	GLfloat sHeight;
	GLfloat legHeight;
	GLfloat legWidth;
	GLint color[3];
	GLint colorBorder[3];


private:
	void DesenhaRect(	GLint height, GLint width,
				GLint forColor[], GLint borColor[]);
	void DrawGun( GLfloat gX, GLfloat gY, GLfloat gTheta1);
	void DrawLegs(GLfloat gX, GLfloat gY);
	void DrawHeadShoulder(GLfloat radiusX, GLfloat radiusY);
	void DrawPlayer(GLfloat gX, GLfloat gY,	GLfloat shoulderWidth, GLfloat shoulderHeight,
		 		GLfloat gTheta1, GLfloat gTheta2, GLfloat gTheta3, GLfloat headRadius);

public:

	GLfloat gTheta1;

	Robo() {
		gX = 0;
		gY = 0;
		gTheta1 = 0;
		gTheta2 = 0;
		gTheta3 = 0;
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
			DrawPlayer(gX, gY, sWidth, sHeight, gTheta1, gTheta2, gTheta3, hRadius);
	};

	void rotateGun(GLfloat dTheta) {
		this->gTheta1 += dTheta;
	};

	// void MoveRoboX(GLfloat gX) {
	// 	this->gX += gX;
	// 	gThetaWheel -= gX*360/(2*M_PI*radiusWheel);
	// };

	void MoveRoboY(GLfloat gY) {
		this->gY += gY;
	};

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
