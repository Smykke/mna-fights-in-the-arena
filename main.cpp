#include <GL/glut.h>
#include <stdio.h>
#include "player.h"
#include "circle.h"
#include <math.h>
#include <vector>
#include "tinyxml2/tinyxml2.h"

// #define ViewingWidth 500
// #define ViewingHeight 500
int ViewingWidth;
int ViewingHeight;

float gx,gy;
float playerSpeed;
float shotSpeed;
int keys[10];
int mX = 0;
int previousTime;
int frameTime = 0;
float switchTime;

// Player
// bool isJumping = false;
// bool isInside = false;
// GLfloat maxRadius;
// GLfloat originalRadius;
Player player;
// Arena

Circle track;
Circle inside;
std::vector<Circle> high;
std::vector<Circle> low;

// Leitura do arquivo de configuracao
int readConfig (char* input);
void drawCircle (GLfloat x, GLfloat y, GLfloat rd, GLfloat r, GLfloat g, GLfloat b);
int move (float dx, float dy);

void display(void)
{
	/* Limpar todos os pixels */
	glClear (GL_COLOR_BUFFER_BIT);

  // Desenhar arena
  drawCircle(track.cx, track.cy, track.radius, track.r, track.g, track.b);
  drawCircle(inside.cx, inside.cy, inside.radius, inside.r, inside.g, inside.b);
  for (int i = 0; i < high.size(); i++) {
    drawCircle(high[i].cx, high[i].cy, high[i].radius, high[i].r, high[i].g, high[i].b);
  }
  for (int i = 0; i < low.size(); i++) {
    drawCircle(low[i].cx, low[i].cy, low[i].radius, low[i].r, low[i].g, low[i].b);
  }

  // Desenhar o jogador
	player.jump(previousTime, low);
	player.Draw();

  /* Não esperar! */
	glFlush ();
}

void keyPress(unsigned char key, int x, int y)
{
	switch(key){
		case 'w':
		case 'W': keys[0] = 1; // foward
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
			if (!player.jumping) {
				player.jumping = true;
				previousTime = glutGet(GLUT_ELAPSED_TIME);
			}
			break;
 		case 27 :
			exit(0);
	default	: 	break;
	}
}

void idleFunc(void)
{
  int now = glutGet(GLUT_ELAPSED_TIME);
  if (now - frameTime >= 0.001) {
  	if(keys[0])	{
      move(0, -playerSpeed);
  	}
  	if(keys[1])	{
      move(0, playerSpeed);
  	}
  	if(keys[2]) {
  		if(keys[0] || keys[1]) {
        player.RotatePlayer(-playerSpeed/10);
        move(-playerSpeed/2, 0);
      } else player.RotatePlayer(-playerSpeed/2);
  	}
  	if(keys[3]) {
  		if(keys[0] || keys[1]) {
        player.RotatePlayer(playerSpeed/10);
        move(playerSpeed/2, 0);
      } else player.RotatePlayer(playerSpeed/2);
  	}
  	glutPostRedisplay();
    frameTime = now;
  }
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
	if ( (x - mX > 0) && (player.gThetaGun < 45) ) player.rotateGun(1);	// left
	else if ( (x - mX < 0) && (player.gThetaGun > -45) ) player.rotateGun(-1);	// right
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
	glClearColor (1.0, 1.0, 1.0, 1.0);
	/* inicializar sistema de viz. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glOrtho(track.cx - track.radius, track.cx + track.radius, track.cy + track.radius, track.cy - track.radius, -1.0, 1.0);
}

int main(int argc, char** argv)
{
  bool read;
  if (argv[1] != NULL) read = readConfig(argv[1]);
  else read = readConfig(0);

  if (read) {
    printf("Ready to launch, Commander.\n");

    // OPENGL
  	glutInit(&argc, argv);
  	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  	glutInitWindowSize (2*track.radius, 2*track.radius);
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
  }
  else printf("Houston, we have a problem.\n");


	return 0;
}

/******** TRATAMENTO DE COLISOES ********/

int move (float dx, float dy) {
  GLfloat nextX = player.gX + dx;
  GLfloat nextY = player.gY + dy;

  if (getPlayerDistance(nextX, nextY, inside) - player.hRadius >= inside.radius) {
    if (getPlayerDistance(nextX, nextY, track) + player.hRadius < track.radius) {
      for (int i = 0; i < high.size(); i++) {
        if (getPlayerDistance(nextX, nextY, high[i]) <= (player.hRadius + high[i].radius)) {
          return 0;
        }
      }
      for (int i = 0; i < low.size(); i++) {
        if (getPlayerDistance(nextX, nextY, low[i]) < (low[i].radius + player.hRadius)) {
          if (player.inside && !player.jumping) {
            if (getPlayerDistance(nextX, nextY, low[i]) > (low[i].radius)) {
              return 0;
            }
          }
          if (!player.inside && !player.jumping) return 0;
        }
      }
      player.MovePlayerX(dx);
      player.MovePlayerY(dy);
      if (player.twoSeconds(switchTime) <= 0.5) {
        player.switchLeg = true;
      } else if (player.twoSeconds(switchTime) < 1) {
        player.switchLeg = false;
      } else switchTime = glutGet(GLUT_ELAPSED_TIME);
    }
  }

}


/******** LEITURA DO ARQUIVO DE CONFIGURACAO ********/
int readConfig (char* input) {

  tinyxml2::XMLDocument doc;
  // CAMINHO DO ARQUIVO DE ENTRADA
  if (input != 0) {
    char filePath[strlen(input) + 10];
    strcpy(filePath, input);
    strcat(filePath, "config.xml");
    printf("Arquivo de configuracao: %s\n", filePath);
    doc.LoadFile(filePath);
  }
  else doc.LoadFile("config.xml");

  if (doc.ErrorID() == 0) printf("Arquivo de configuracao carregado!\n");
  else {
    printf("Erro na leitura do arquivo de configuracao.\n");
    return 0;
  }

  const char * arenaAtr1;
  const char * arenaAtr2;
  const char * arenaAtr3;
  tinyxml2::XMLElement * arena = doc.FirstChildElement( "aplicacao" )->FirstChildElement( "arquivoDaArena" );

  arenaAtr1 = arena->Attribute( "nome" );
  arenaAtr2 = arena->Attribute( "tipo" );
  arenaAtr3 = arena->Attribute( "caminho" );
  // printf("Local da arena: ");
  // if (arenaAtr3 != NULL) printf("%s", arenaAtr3);
  // printf("%s.%s\n", arenaAtr1, arenaAtr2);

  int tam = strlen(arenaAtr1) + strlen(arenaAtr2);
  if (arenaAtr3 != NULL) tam += strlen(arenaAtr3);

  char filePath[tam];
  if (arenaAtr3 != NULL) strcpy(filePath, arenaAtr3);
  strcat(filePath, arenaAtr1); strcat(filePath, "."); strcat(filePath, arenaAtr2);
  printf("Path da arena: %s\n", filePath);

  arena = arena->NextSiblingElement("jogador");
  arena->QueryFloatAttribute( "vel", &playerSpeed );
  // printf("Velocidade do jogador: %f\n", playerSpeed);
  arena->QueryFloatAttribute( "velTiro", &shotSpeed );

  doc.LoadFile(filePath);
  if (doc.ErrorID() == 0) printf("Arquivo de arena carregado!\n");
  else {
    printf("Erro na leitura do arquivo de arena.\n");
    return 0;
  }

  arena = doc.FirstChildElement("svg")->FirstChildElement("circle");
  int j = 0;

  while (arena != NULL) {
    if (strcmp(arena->Attribute( "fill" ), "green") == 0) {
      GLfloat playerInfo[3];
      arena->QueryFloatAttribute( "cx", &playerInfo[0] );
      arena->QueryFloatAttribute( "cy", &playerInfo[1] );
      arena->QueryFloatAttribute( "r", &playerInfo[2] );
      player = Player(playerInfo[0], playerInfo[1], playerInfo[2]);
    }
    else {
      Circle tmp;
      arena->QueryFloatAttribute( "cx", &tmp.cx );
      arena->QueryFloatAttribute( "cy", &tmp.cy );
      arena->QueryFloatAttribute( "r", &tmp.radius );
      const char * color = arena->Attribute("fill");
      if (strcmp(color, "blue") == 0) {
        tmp.b = 1.0;
        tmp.g = 0.0;
        tmp.r = 0.0;
        track = tmp;
      }
      else {
        if (strcmp(color, "white") == 0) {
          tmp.r = 1.0;
          tmp.g = 1.0;
          tmp.b = 1.0;
          inside = tmp;
        }
        else {
          if (strcmp(color, "red") == 0) {
            tmp.r = 1.0;
            tmp.g = 0.0;
            tmp.b = 0.0;
            high.push_back(tmp);
          } // red
          else {
            tmp.r = 0.0;
            tmp.g = 0.0;
            tmp.b = 0.0;
            low.push_back(tmp);
          }
        }
      }
    }
    arena = arena->NextSiblingElement("circle");
  }

  return 1;
}

/******** FUNCAO PARA DESENHAR A ARENA ********/
void drawCircle (GLfloat x, GLfloat y, GLfloat rd, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f (r, g, b);

  glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * M_PI; i += M_PI/18)
      glVertex3f( cos(i) * rd + x, sin(i) * rd + y, 0.0 );
  glEnd();
}
