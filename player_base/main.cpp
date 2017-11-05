#include <GL/glut.h>
#include <stdio.h>
#include "robo.h"
#define ViewingWidth 500
#define ViewingHeight 500

float gx,gy;
int keys[10];
Robo robo;
int mX = 0;
bool isJumping = false;
int previousTime;
GLfloat maxRadius;
GLfloat originalRadius;

float twoSeconds() {
  /* Delta time in seconds. */
  int t;
  float dt;
  t = glutGet(GLUT_ELAPSED_TIME);
  dt = (t - previousTime) / 1000.0;
  return dt;
}

void jump () {
    if (isJumping) {
      if (twoSeconds() >= 2) {
        robo.ResizePlayer(originalRadius);
        isJumping = false;
			}
			else {
        if (twoSeconds() <= 1) robo.ResizePlayer(originalRadius*(1 + twoSeconds()*0.5));
        else robo.ResizePlayer(maxRadius - (twoSeconds()-1)*0.5*originalRadius);
    	}
		}
}


void display(void)
{
	/* Limpar todos os pixels */
	glClear (GL_COLOR_BUFFER_BIT);
	jump();
	robo.Draw();
	/* Não esperar! */
	glFlush ();
}
void keyPress(unsigned char key, int x, int y)
{
	switch(key){
		case 'w':
		case 'W': 	keys[0] = 1; // foward
			break;
		case 's':
		case 'S':	keys[1] = 1;	// backward
			break;
		case 'a':
		case 'A':	keys[2] = 1; // anti-clockwise
				break;
		case 'd':
		case 'D':	keys[3] = 1;	// clockwise
				break;
		case 'p':
 		case 'P':
			if (!isJumping) {
				isJumping = true;
				previousTime = glutGet(GLUT_ELAPSED_TIME);
				printf("Pressionou p\n");
			}
			break;
 		case 27 :
			exit(0);
	default	: 	break;
	}
}

void idleFunc(void)
{
	if(keys[0])	{
		robo.MovePlayerY(0.1);
		// if (robo.gThetaPl > 0) robo.MovePlayerX(-0.05);
		// else if(robo.gThetaPl < 0) robo.MovePlayerX(0.05);
	}
	if(keys[1])	{
		robo.MovePlayerY(-0.1);
		// if (robo.gThetaPl > 0) robo.MovePlayerX(-0.05);
		// else if(robo.gThetaPl < 0) robo.MovePlayerX(0.05);
	}
	if(keys[2]) {
		robo.RotatePlayer(0.005);
		if(keys[0] || keys[1]) robo.MovePlayerX(-0.05);
	}
	if(keys[3]) {
		robo.RotatePlayer(-0.005);
		if(keys[0] || keys[1]) robo.MovePlayerX(0.05);
	}


	glutPostRedisplay();
}

void keyRelease(unsigned char key, int x, int y)
{
	switch(key){
		case 'w':
		case 'W': 	keys[0] = 0;	// foward
			break;
		case 's':
		case 'S':	keys[1] = 0;	// back
			break;
		case 'a':
		case 'A':	keys[2] = 0;	// anti-clockwise
			break;
		case 'd':
		case 'D':	keys[3] = 0;	// clockwise
			break;
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
	if ( (x - mX > 0) && (robo.gThetaGun > -45) ) robo.rotateGun(-0.5);	// left
	else if ( (x - mX < 0) && (robo.gThetaGun < 45) ) robo.rotateGun(0.5);	// right
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
	maxRadius = robo.hRadius*1.5;
	originalRadius = robo.hRadius;
	printf("maxR: %.2f, oriR: %.2f\n", maxRadius, originalRadius);
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
