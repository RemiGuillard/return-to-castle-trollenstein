#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

// Personnal includes
#include "Trollenstein.h"

#ifndef _MAZE_
#define _MAZE_

#define near 0.1
#define far 500.0
#define right 0.1
#define left -0.1
#define top 0.1
#define bottom -0.1

#define M_PI 3.14409

void	lookAt(GLfloat px, GLfloat py, GLfloat pz,
			   GLfloat lx, GLfloat ly, GLfloat lz,
			   GLfloat vx, GLfloat vy, GLfloat vz,
			   GLfloat *m);

void	rotationVectX(float angle, GLfloat *A, GLfloat *B);
void	rotationVectY(float angle, GLfloat *A, GLfloat *B);

#endif //!_MAZE_
