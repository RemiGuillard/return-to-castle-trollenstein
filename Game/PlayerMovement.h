#ifndef _MOVEMENT_
# define _MOVEMENT_

#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

// Personnal includes
#include "Trollenstein.h"
#include "MazeUtilities.h"
#include "castle.h"


extern GLfloat	position[];
extern GLfloat	look[];
extern int		flying;
extern float	angh;
extern float	angv;
extern mapData	*map;

void moveThePlayer();

#endif //!_MOVEMENT_
