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
Model *bunnyModel;
Model *teddyModel;

// Texture
GLuint	*flowerTex;
GLuint	*bilsTex;



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
  program = loadShaders("lab2-5.vert", "lab2-5.frag");
  printError("init shader");


  bunnyModel = LoadModelPlus("bunnyplus.obj",program, "inPos", "inNorm","inTex");
  teddyModel = LoadModelPlus("models/various/teddy.obj",program, "inPos", "inNorm","inTex");

  printError("init arrays1");

  LoadTGATextureSimple("maskros512.tga", &flowerTex);
  printError("init Tex1");

  LoadTGATextureSimple("bilskissred.tga", &bilsTex);
  printError("init Tex2");
}


void display(void)
{
  float t = glutGet(GLUT_ELAPSED_TIME);

  GLfloat rotationMatrix[16];
  GLfloat translationMatrix[16];

  IdentityMatrix(rotationMatrix);
  IdentityMatrix(translationMatrix);
  
  translationMatrix[3] = 1.0;

  GLfloat cam[16];
  lookAt(cos(t/1000)*3, 2.0, -sin(t/1000)*3,
	 0.0, 0.0, 0.0,
	 0.0, 1.0, 0.0,
	 cam);

  printError("pre display");

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Send Translation
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);
  // Send projection matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
  // Send translation matrix

  // Bind for bunny
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrix);
  glBindTexture(GL_TEXTURE_2D, flowerTex);
  glUniform1i(glGetUniformLocation(program, "Texture"), 0); // Texture unit 0 */

  DrawModel(bunnyModel);


  // Bind for teddy
  translationMatrix[3] = -1.0;
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrix);
  glBindTexture(GL_TEXTURE_2D, bilsTex);
  glUniform1i(glGetUniformLocation(program, "Texture"), 0); // Texture unit 0 */

  DrawModel(teddyModel);



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
