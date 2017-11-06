#ifndef ARENA_H
#define ARENA_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "player.h"
#include <math.h>
#include <vector>
#include "tinyxml2/tinyxml2.h"


typedef struct circle {
  GLfloat cx, cy, radius;
  GLfloat r, g, b;
} Circle;

void drawCircle (GLfloat x, GLfloat y, GLfloat rd, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f (r, g, b);

  glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * M_PI; i += M_PI/18)
      glVertex3f( cos(i) * rd + x, sin(i) * rd + y, 0.0 );
  glEnd();
}

/******** LEITURA DO ARQUIVO DE CONFIGURACAO ********/
int readConfig (char* input, Circle track, Circle inside, std::vector<Circle> high, std::vector<Circle> low, Player player) {

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

#endif
