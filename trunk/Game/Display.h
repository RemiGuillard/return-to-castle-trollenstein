#ifndef _DISPLAY_
# define _DISPLAY_

#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

// Personnal includes
#include "Trollenstein.h"
#include "MazeUtilities.h"
#include "PlayerMovement.h"

//MAP DATA
extern mapData *map;

// Angle & pos matrix
extern GLfloat position[];
extern GLfloat look[];
extern Point3D warpPos;

//Window Size
extern int height;
extern int width;

// vertex array object
extern Model *ground, *wall, *skybox, *warp;
// Reference to shader program
extern GLuint program, skyprog;
extern GLuint texGround, texWall, texSky, texWarp;


void display();

#endif
