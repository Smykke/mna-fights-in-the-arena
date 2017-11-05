#include "robo.h"

void Robo::DesenhaRect(	GLint width, GLint height,
			GLint forColor[], GLint borColor[])
{
	glColor3f(forColor[0], forColor[1], forColor[2]); // Interior
	glBegin(GL_QUADS);
		glVertex2f( -width/2.0,0.0);
		glVertex2f( -width/2.0,height);
		glVertex2f( width/2.0,height);
		glVertex2f( width/2.0,0.0);
	glEnd();

	glColor3f(borColor[0], borColor[1], borColor[2]); // Borda
	glLineWidth(1.0);
	glBegin(GL_LINES);
		glVertex2f( -width/2.0,0.0);
		glVertex2f( -width/2.0,height);
		glVertex2f( width/2.0,height);
		glVertex2f( width/2.0,0.0);
	glEnd();
}

void Robo::DrawLegs( GLfloat gX, GLfloat gY) { // verificar: colocar a cor no robo?
	glPushMatrix();
	glTranslatef(gX, gY, 0);
	// glRotatef(gTheta1, 0, 0, 1);	// rotacionar o eixo z (mexer x, y)
	DesenhaRect(this->legWidth, this->legHeight, this->colorBorder, this->colorBorder);

	glTranslatef(this->hRadius, gY, 0);
	DesenhaRect(this->legWidth, -(this->legHeight), this->colorBorder, this->colorBorder);

	glPopMatrix();
}

void Robo::DrawGun( GLfloat gX, GLfloat gY, GLfloat gTheta1) {
	glPushMatrix();
	glTranslatef(gX, gY, 0);
	glRotatef(gTheta1, 0, 0, 1);	// rotacionar o eixo z (mexer x, y)
	DesenhaRect(legWidth, legHeight, this->color, this->colorBorder);

	// verificar: bala tem que partir daqui?

	glPopMatrix();
}

void Robo::DrawHeadShoulder(GLfloat radiusX, GLfloat radiusY) {
	int i;
	GLfloat x, y;
	glColor3f(this->color[0], this->color[1], this->color[2]); // Interior
	glBegin(GL_POLYGON);
		for (i = 0; i < 360; i += 20) {
			x = radiusX * cos(M_PI*i/180);
			y = radiusY * sin(M_PI*i/180);
			glVertex3f(x, y, 0);
		}
	glEnd();

	glColor3f(this->colorBorder[0], this->colorBorder[1], this->colorBorder[2]);
	glPointSize(1);
	glBegin(GL_POINTS); // Borda
		for (i = 0; i < 360; i += 1) {
			x = radiusX * cos(M_PI*i/180);
			y = radiusY * sin(M_PI*i/180);
			glVertex3f(x, y, 0);
		}
	glEnd();
}

void Robo::DrawPlayer(GLfloat gX, GLfloat gY,	GLfloat shoulderWidth, GLfloat shoulderHeight,
			GLfloat gTheta1, GLfloat gTheta2, GLfloat gTheta3, GLfloat headRadius)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(gX,gY,0);
	DrawGun(3*headRadius/2, 0, gTheta1);
	DrawLegs(-(headRadius/2.0), 0);
	DrawHeadShoulder(shoulderWidth, shoulderHeight); // Ombro fica embaixo
	DrawHeadShoulder(headRadius, headRadius);	// Desenha a cabeca em seguida

	// Roda direita
	// glPushMatrix();
	// glTranslatef(baseWidth/2, 0, 0);
	// glRotatef(gThetaWheel, 0, 0, 1);
	// glPopMatrix();

	// //Roda esquerda
	// glPushMatrix();
	// glTranslatef(-(baseWidth/2), 0, 0);
	// glRotatef(gThetaWheel, 0, 0, 1);
	// DesenhaCirc(radiusWheel, 0, 1, 1);
	// glPopMatrix();
	//
	glPopMatrix();
}
