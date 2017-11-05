#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include "tinyxml2/tinyxml2.h"

int windowWidth;
int windowHeight;
int keyStatus[256];
bool isJumping = false;
bool isInside = false;
int previousTime;
GLfloat maxRadius;
GLfloat originalRadius;

typedef struct circle {
  GLfloat cx, cy, radius;
  GLfloat r, g, b;
} Circle;

Circle player;
Circle track;
Circle inside;
std::vector<Circle> high;
std::vector<Circle> low;

int readConfig (char* input);

/******** MODIFICACOES NO CIRCULO DO JOGADOR ********/
float getPlayerDistance(float pX, float pY, Circle obj) {
  return sqrt(pow(pX - obj.cx, 2) + pow(pY - obj.cy, 2));
}

int move (float dx, float dy) {
  GLfloat nextX = player.cx + dx;
  GLfloat nextY = player.cy + dy;

  if (getPlayerDistance(nextX, nextY, inside) - player.radius >= inside.radius) {
    if (getPlayerDistance(nextX, nextY, track) + player.radius < track.radius) {
      for (int i = 0; i < high.size(); i++) {
        if (getPlayerDistance(nextX, nextY, high[i]) <= (player.radius + high[i].radius)) {
          return 0;
        }
      }
      for (int i = 0; i < low.size(); i++) {
        if (getPlayerDistance(nextX, nextY, low[i]) < (low[i].radius + player.radius)) {
          if (isInside && !isJumping) {
            if (getPlayerDistance(nextX, nextY, low[i]) + player.radius > (low[i].radius)) {
              return 0;
            }
          }
          if (!isInside && !isJumping) return 0;
        }
      }
      player.cx = nextX;
      player.cy = nextY;
    }
  }

}

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
        player.radius = originalRadius;
        isJumping = false;
        for (int i = 0; i < low.size(); i++) {
          if (getPlayerDistance(player.cx, player.cy, low[i]) + player.radius < (low[i].radius)) {
            isInside = true;
            break;
          }
          else isInside = false;
        }
      }
      else {
        if (twoSeconds() <= 1) player.radius = originalRadius*(1 + twoSeconds()*0.5);
        else player.radius = maxRadius - (twoSeconds()-1)*0.5*originalRadius;
      }
    }
}


/******** DESENHO DA TELA ********/
void drawCircle (GLfloat x, GLfloat y, GLfloat rd, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f (r, g, b);

  glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * M_PI; i += M_PI/18)
      glVertex3f( cos(i) * rd + x, sin(i) * rd + y, 0.0 );
  glEnd();
}

void display(void)
{
  // Limpar todos os pixels
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
  jump();
  drawCircle(player.cx, player.cy, player.radius, player.r, player.g, player.b);

  /* Não esperar! */
  glutSwapBuffers ();

}


/******** CONTROLE DO TECLADO ********/
void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
        case 'W':
             keyStatus[(int)('w')] = 1; //Using keyStatus trick
             break;
        case 's':
        case 'S':
             keyStatus[(int)('s')] = 1;
             break;
        case 'a':
        case 'A':
             keyStatus[(int)('a')] = 1; //Using keyStatus trick
             break;
        case 'd':
        case 'D':
             keyStatus[(int)('d')] = 1; //Using keyStatus trick
             break;
        case 'p':
        case 'P':
             if (!isJumping) {
               isJumping = true;
               previousTime = glutGet(GLUT_ELAPSED_TIME);
             }
             break;
        case 27 :
             exit(0);
    }
    glutPostRedisplay();
}

void idle(void)
{
    //Treat keyPress
    if(keyStatus[(int)('a')])
    {
        // player.cx -= 0.1;
        move (-1, 0);
    }
    if(keyStatus[(int)('d')])
    {
        // player.cx += 0.1;
        move (1, 0);
    }
    if(keyStatus[(int)('w')])
    {
        //  player.cy += 1;
        move (0, -1);
    }
    if(keyStatus[(int)('s')])
    {
        //  player.cy -= 1;
        move (0, 1);
    }

    glutPostRedisplay();
}


/******** OPENGL ********/
void init (void)
{
  /* selecionar cor de fundo */
  glClearColor (1.0, 1.0, 1.0, 0.0);

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
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (2*track.radius, 2*track.radius);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("arena");
    init ();
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyup);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
  }
  else printf("Houston, we have a problem.\n");

  return 0;
}



// LEITURA DO ARQUIVO DE CONFIGURACAO
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
      arena->QueryFloatAttribute( "cx", &player.cx );
      arena->QueryFloatAttribute( "cy", &player.cy );
      arena->QueryFloatAttribute( "r", &player.radius );
      originalRadius = player.radius;
      maxRadius = player.radius*1.5;
      player.r = 0.0;
      player.g = 1.0;
      player.b = 0.0;
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
