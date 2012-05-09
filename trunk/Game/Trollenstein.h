#ifndef _TROLLENSTEIN_H
# define _TROLLENSTEIN_H

#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

#define WALL '1'
#define START '2'
#define WARP '3'

#define ANGMOV 5.0

typedef struct {

  int width;
  int height;

  char **map;
  
} mapData;

mapData *loadMap(char* fileName);
Model*	GenerateGround(mapData *map,
					   GLuint program,
					   char* vertexVariableName,
					   char* normalVariableName,
					   char* texCoordVariableName);

int		getNbWalls(mapData *map);

#endif //!_TROLLENSTEIN_H
