#ifndef _TROLLENSTEIN_H
# define _TROLLENSTEIN_H

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

#endif //!_TROLLENSTEIN_H
