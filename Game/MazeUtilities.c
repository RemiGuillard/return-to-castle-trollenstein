#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

// Personnal includes
#include "Trollenstein.h"
#include "MazeUtilities.h"

void lookAt(GLfloat px, GLfloat py, GLfloat pz,
	    GLfloat lx, GLfloat ly, GLfloat lz,
	    GLfloat vx, GLfloat vy, GLfloat vz,
	    GLfloat *m)
{
  Point3D p, l, n, v, u;
  GLfloat r[16], t[16];
	
  SetVector(px, py, pz, &p);
  SetVector(lx, ly, lz, &l);
  SetVector(vx, vy, vz, &v);
	
  VectorSub(&p, &l, &n); // backwards vector
  Normalize(&n);
	
  CrossProduct(&v, &n, &u); // right vector
  Normalize(&u);
	
  CrossProduct(&n, &u, &v); // orthogonal up vector
	
  T(-px, -py, -pz, t);
	
  r[0] = u.x; r[1] = u.y; r[2] = u.z;  r[3] = 0;
  r[4] = v.x; r[5] = v.y; r[6] = v.z;  r[7] = 0;
  r[8] = n.x; r[9] = n.y; r[10] = n.z; r[11] = 0;
  r[12] = 0;   r[13] = 0;   r[14] = 0;   r[15] = 1;
	
  Mult(r, t, m);
}

float getRad(float angle) {
  float temp;

  temp = angle * M_PI;
  temp /= 180.0;

  return temp;
}

float getAngle(float angle) {
  float temp;

  temp = angle * 180.0;
  temp /=  M_PI;

  return temp;
}


void rotationVectY(float angle, GLfloat *A, GLfloat *B) {

  Point3D AB;
  float xprim; float yprim;

  SetVector(B[0] - A[0], B[1] - A[1], B[2] - A[2], &AB);
  xprim = AB.x * cos(getRad(angle)) - AB.z * sin(getRad(angle));
  yprim = AB.x * sin(getRad(angle)) + AB.z * cos(getRad(angle));

  B[0] = xprim + A[0];
  B[2] = yprim + A[2];
}

void rotationVectX(float angle, GLfloat *A, GLfloat *B) {

  Point3D AB;
  float xprim; float yprim;

  SetVector(B[0] - A[0], B[1] - A[1], B[2] - A[2], &AB);
  Normalize(&AB);
  xprim = AB.y * cos(getRad(angle)) - AB.z * sin(getRad(angle));
  yprim = AB.y * sin(getRad(angle)) + AB.z * cos(getRad(angle));

  B[1] = xprim + A[1];
  B[2] = yprim + A[2];
}
