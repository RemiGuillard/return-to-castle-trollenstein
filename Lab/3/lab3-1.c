// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include "GL_utilities.h"
#include "loadobj.h"
#include "VectorUtils2.h"

// Globals
// Data would normally be read from files

#define M_PI 3.14409

//Projection matrix
#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
				0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
				0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
				0.0f, 0.0f, -1.0f, 0.0f };

// vertex array object
unsigned int bunnyVertexArrayObjID;

// Shaders 
GLuint program;

// Model
Model *walls;
Model *blade;
Model *balcony;
Model *roof;


void OnTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(20, &OnTimer, value);
}



/////// LOOK AT ////////
////////////////////////

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

  // initialize r, t, m as matrices
  IdentityMatrix(r);
  IdentityMatrix(t);
  IdentityMatrix(m);

  // Calc n and normalize it
  VectorSub(&l, &p, &n);
  Normalize(&n);
  n.x = -n.x;
  n.y = -n.y;
  n.z = -n.z;

  // Calc cross product u of v and n and normalize it
  CrossProduct(&v, &n, &u);
  Normalize(&u);

  // cross product v of n and u
  CrossProduct(&n, &u, &v);

  // Put result in r matrice
  r[0] = u.x;  r[1] = u.y;  r[2] = u.z;
  r[4] = v.x;  r[5] = v.y;  r[6] = v.z;
  r[8] = n.x;  r[9] = n.y;  r[10] = n.z;

  //put translation coord in t
  t[3] = -p.x;
  t[7] = -p.y;
  t[11] = -p.z;
  
  // do operation r.t => result in m
  Mult(r, t, m);
  //int i;
  //for(i=0;i<16;i++) printf("%f ",m[i]);
}

/// END LOOK AT //////
//////////////////////



void init(void)
{
  // two vertex buffer objects, used for uploading the

  // Reference to shader program

  dumpInfo();

  // GL inits
  glClearColor(0.2,0.2,0.5,0);
  glEnable(GL_DEPTH_TEST);
  printError("GL inits");

  // Load and compile shader
  program = loadShaders("lab3-1.vert", "lab3-1.frag");
  printError("init shader");


  roof = LoadModelPlus("windmill/windmill-roof.obj",program, "inPos", "inNorm","inTex");
  walls = LoadModelPlus("windmill/windmill-walls.obj",program, "inPos", "inNorm","inTex");
  balcony = LoadModelPlus("windmill/windmill-balcony.obj",program, "inPos", "inNorm","inTex");
  blade = LoadModelPlus("windmill/blade.obj",program, "inPos", "inNorm","inTex");
  printError("init objects");

}


void display(void)
{
  float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

  GLfloat roofMatrix[16];
  GLfloat balconyMatrix[16];
  GLfloat wallsMatrix[16];
  GLfloat bladeMatrix[16];

  IdentityMatrix(roofMatrix);
  IdentityMatrix(balconyMatrix);
  IdentityMatrix(wallsMatrix);
  IdentityMatrix(bladeMatrix);
  
  GLfloat cam[16];
  lookAt(cos(t/5) * 20, 17, -sin(t/5) * 20,
	 0.0, 7.0, 0.0,
	 0.0, 1.0, 0.0,
	 cam);

  printError("pre display");

  bladeMatrix[3] = 4.6;
  bladeMatrix[7] = 9.3;

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Send Translation
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);
  // Send projection matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
  // Send translation matrix

  // Bind for walls
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, wallsMatrix);
  DrawModel(walls);

  // Bind for roof
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, roofMatrix);
  DrawModel(roof);

  // Bind for balcony
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, balconyMatrix);
  DrawModel(balcony);

  // Bind for blade1
  bladeMatrix[5] = cos(0 * M_PI / 180 + t ); bladeMatrix[6] = -sin(0 * M_PI / 180 + t );
  bladeMatrix[9] = sin(0 * M_PI / 180 + t ); bladeMatrix[10] = cos(0 * M_PI / 180 + t );
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladeMatrix);
  DrawModel(blade);

  // Bind for blade2
  bladeMatrix[5] = cos(90 * M_PI / 180 + t); bladeMatrix[6] = -sin(90 * M_PI / 180 + t);
  bladeMatrix[9] = sin(90 * M_PI / 180 + t); bladeMatrix[10] = cos(90 * M_PI / 180 + t);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladeMatrix);
  DrawModel(blade);

  // Bind for blade3
  bladeMatrix[5] = cos(180 * M_PI / 180 + t); bladeMatrix[6] = -sin(180 * M_PI / 180 + t);
  bladeMatrix[9] = sin(180 * M_PI / 180 + t); bladeMatrix[10] = cos(180 * M_PI / 180 + t);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladeMatrix);
  DrawModel(blade);

  // Bind for blade4
  bladeMatrix[5] = cos(270 * M_PI / 180 + t); bladeMatrix[6] = -sin(270 * M_PI / 180 + t);
  bladeMatrix[9] = sin(270 * M_PI / 180 + t); bladeMatrix[10] = cos(270 * M_PI / 180 + t);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, bladeMatrix);
  DrawModel(blade);


  printError("display");

  glutSwapBuffers();
}


int main(int argc, const char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("Glu look at the rabbit");
  glutDisplayFunc(display);
  init ();
  glutTimerFunc(20, &OnTimer, 0);
  glutMainLoop();
}
