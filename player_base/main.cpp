#include <GL/glut.h>
#include <stdio.h>
#include "robo.h"
#define ViewingWidth 500
#define ViewingHeight 500

float gx,gy;
int keys[10];
Robo robo;
int mX = 0;

void display(void)
{
	/* Limpar todos os pixels */
	glClear (GL_COLOR_BUFFER_BIT);
	/* Desenhar um polígono branco (retângulo) */
	robo.Draw();
	/* Não esperar! */
	glFlush ();
}
void keyPress(unsigned char key, int x, int y)
{
	switch(key){
		case 'w': 	keys[0] = 1; // foward
		break;
		case 's':	keys[1] = 1;	// back
		break;
	// case 'a':	keys[0] = 1;
	// 		break;
	// case 'd':	keys[1] = 1;
	// 		break;
	// case 'r': 	keys[4] = 1;
	// 		break;
	// case 'f': 	keys[5] = 1;
	// 		break;
	// case 't': 	keys[6] = 1;
	// 		break;
	// case 'g': 	keys[7] = 1;
	// 		break;
	// case 'y': 	keys[8] = 1;
	// 		break;
	// case 'h': 	keys[9] = 1;
	// 		break;
	default	: 	break;
	}
}

void idleFunc(void)
{
	if(keys[0])	robo.MoveRoboY(0.1);
	if(keys[1])	robo.MoveRoboY(-0.1);
	// if(keys[0]) robo.MoveRoboX(-0.1);
	// if(keys[1]) robo.MoveRoboX(0.1);
	// if(keys[4])	robo.RodaBraco1(0.05);
	// if(keys[5])	robo.RodaBraco1(-0.05);
	// if(keys[6])	robo.RodaBraco2(0.05);
	// if(keys[7])	robo.RodaBraco2(-0.05);
	// if(keys[8])	robo.RodaBraco3(0.05);
	// if(keys[9])	robo.RodaBraco3(-0.05);
	glutPostRedisplay();
}

void keyRelease(unsigned char key, int x, int y)
{
	switch(key){
		case 'w': 	keys[0] = 0;	// foward
		break;
		case 's':	keys[1] = 0;	// back
		break;
	// case 'a':	keys[0] = 0;
	// 		break;
	// case 'd':	keys[1] = 0;
	// 		break;
	// case 'r': 	keys[4] = 0;
	// 		break;
	// case 'f': 	keys[5] = 0;
	// 		break;
	// case 't': 	keys[6] = 0;
	// 		break;
	// case 'g': 	keys[7] = 0;
	// 		break;
	// case 'y': 	keys[8] = 0;
	// 		break;
	// case 'h': 	keys[9] = 0;
	// 		break;
	default	: 	break;
	}
}

void mouseClick(int button, int state, int x, int y)
{
	y = 250-y;
	printf("button: %d\nx: %d y: %d\n",button,x,y);
	gx = (GLfloat)x/250;
	gy = (GLfloat)y/250;
}

void mouseDrag (int x, int y) {
	y = 250 - y; // verificar: tamanho da janela
	if ( (x - mX > 0) && (robo.gTheta1 > -45) ) robo.rotateGun(-0.5);
	else if ( (x - mX < 0) && (robo.gTheta1 < 45) ) robo.rotateGun(0.5);
	mX = x;
}

void motion(int x, int y)
{
	y = 250-y;
	gx = (GLfloat)x/250;
	gy = (GLfloat)y/250;
}
void init (void)
{
	/* selecionar cor de fundo (preto) */
	glClearColor (0.0, 0.0, 0.0, 0.0);
	/* inicializar sistema de viz. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(ViewingWidth/2),(ViewingWidth/2),-(ViewingHeight/2),(ViewingHeight/2), 100, -100);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE |
	GLUT_RGB);
	glutInitWindowSize (250, 250);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("player");
	init ();
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutIdleFunc(idleFunc);
	glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseDrag);
	glutMotionFunc(motion);
	glutMainLoop();
	/* C ANSI requer que main retorne um
	inteiro */
	return 0;
}
