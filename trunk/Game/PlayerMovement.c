#include "PlayerMovement.h"

void moveThePlayer() {

  GLfloat orig[] = { position[0], position[1], position[2] };
  GLfloat dir[] = { look[0], look[1], look[2] };

  float move = 0.05;
  
  Point3D AB;

  GLfloat oldPos[] = { position[0], position[1], position[2] };
  GLfloat oldDir[] = { look[0], look[1], look[2] };
  
  if (keyIsDown(27))
    exit(EXIT_SUCCESS);

  if (keyIsDown('p')) {
    if (flying == 1) {
      look[1] -= position[1];
      position[1] = 0.5f;
      flying = 0;
    }
  }

  if (keyIsDown('o'))
    flying = 1;
  if (keyIsDown('z')) { //UP
    SetVector(look[0] - position[0], look[1] - position[1], look[2] - position[2], &AB);
    Normalize(&AB);
    look[0] += move * AB.x;
    look[2] += move * AB.z;
    position[0] += move * AB.x;
    position[2] += move * AB.z;
    if (flying == 1) {
      look[1] += move * AB.y;
      position[1] += move * AB.y;
    }
  }
  if (keyIsDown('s')) { //DOWN
    SetVector(look[0] - position[0], look[1] - position[1], look[2] - position[2], &AB);
    Normalize(&AB);
    look[0] -= move * AB.x;
    look[2] -= move * AB.z;
    position[0] -= move * AB.x;
    position[2] -= move * AB.z;
    if (flying == 1) {
      look[1] -= move * AB.y;
      position[1] -= move * AB.y;
    }
  }
  if (keyIsDown('d')) { //RIGHT
    rotationVectY(90, orig, dir);
    SetVector(dir[0] - orig[0], dir[1] - orig[1], dir[2] - orig[2], &AB);
    Normalize(&AB);
    look[0] += move * AB.x;
    look[2] += move * AB.z;
    position[0] += move * AB.x;
    position[2] += move * AB.z;
    if (flying == 1) {
      look[1] += move * AB.y;
      position[1] += move * AB.y;
    }
  }
  if (keyIsDown('q')) { //LEFT
    rotationVectY(90, orig, dir);
    SetVector(dir[0] - orig[0], dir[1] - orig[1], dir[2] - orig[2], &AB);
    Normalize(&AB);
    look[0] -= move * AB.x;
    look[2] -= move * AB.z;
    position[0] -= move * AB.x;
    position[2] -= move * AB.z;
    if (flying == 1) {
      look[1] -= move * AB.y;
      position[1] -= move * AB.y;
    }
  }
  if (keyIsDown('a')) { // Move camera left
    rotationVectY(-ANGMOV, position, look);
    angh -= ANGMOV;
    angh = fmodf(angh, 360.0);
  }
  if (keyIsDown('e')) { // Move camera right
    rotationVectY(ANGMOV, position, look);
    angh += ANGMOV;
    angh = fmodf(angh, 360.0);
  }
  if (keyIsDown('g')) { // Move camera down
    if (angv - ANGMOV > -179.0) {
      if (look[2] > position[2])
	rotationVectX(-ANGMOV, position, look);
      else
	rotationVectX(ANGMOV, position, look);
      angv -= ANGMOV;
    }
  }
  if (keyIsDown('t')) { // move camera up
    if (angv + ANGMOV < 179.0) {
      if (look[2] > position[2])
	rotationVectX(ANGMOV, position, look);
      else
	rotationVectX(-ANGMOV, position, look);
      angv += ANGMOV;
    }
  }
  if (keyIsDown('r')) { // restart the level (player at start)
    setPlayerAtStart();
  }

  if (position[0] < 0)
    position[0] = 0;
  if (position[0] > map->width)
    position[0] = map->width;
  if (position[2] < 0)
    position[2] = 0;
  if (position[2] > map->height)
    position[2] = map->height;
  if (map->map[(int)position[2]][(int)position[0]] == WALL && flying == 0) {
    look[0] = oldDir[0];
    look[2] = oldDir[2];
    position[0] = oldPos[0];
    position[2] = oldPos[2];
  }
  if (map->map[(int)position[2]][(int)position[0]] == WARP) {
    changeMap();
  }
}

