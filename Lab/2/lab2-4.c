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
Model *m;

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
  
  unsigned int	bunnyVertexBufferObjID;
  unsigned int	bunnyIndexBufferObjID;
  unsigned int	bunnyNormalBufferObjID;
  unsigned int	bunnyTexCoordBufferObjID;
  GLuint	*myTex;

  m = LoadModel("bunnyplus.obj");


  // Reference to shader program

  dumpInfo();

  // GL inits
  glClearColor(0.2,0.2,0.5,0);
  glEnable(GL_DEPTH_TEST);
  printError("GL inits");

  // Load and compile shader
  program = loadShaders("lab2-4.vert", "lab2-4.frag");
  printError("init shader");
 
  glGenVertexArrays(1, &bunnyVertexArrayObjID);
  glGenBuffers(1, &bunnyVertexBufferObjID);
  glGenBuffers(1, &bunnyIndexBufferObjID);
  glGenBuffers(1, &bunnyNormalBufferObjID);

  glBindVertexArray(bunnyVertexArrayObjID);
  printError("Bind Vertex array");

  // Load Texture tga
  LoadTGATextureSimple("maskros512.tga", &myTex);

  //Texture init
  if (m->texCoordArray != NULL)
    {
      glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
      glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
      glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
      glBindTexture(GL_TEXTURE_2D, myTex);
      glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
    }
  printError("TexCoord");

  // VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
  glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
  printError("init VertexArrays");

  // VBO for normal data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
  printError("init Normal array");

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
  printError("init arrays");
}


void display(void)
{
  float t = glutGet(GLUT_ELAPSED_TIME);

  GLfloat rotationMatrix[16];
  GLfloat translationMatrix[16];

  IdentityMatrix(rotationMatrix);
  IdentityMatrix(translationMatrix);
  
  /*  rotationMatrix[0] = 0.7;
  rotationMatrix[2] = -0.7;
  rotationMatrix[8] = 0.7;
  rotationMatrix[10] = 0.7;*/
  translationMatrix[11] = 0.0;

  GLfloat cam[16];
  lookAt(3.0, 1.0, 1.0,
	 0.0, 0.0, 0.0,
	 0.0, 1.0, 0.0,
	 cam);

  printError("pre display");

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Send Translation
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrix/*rotationMatrix*/);
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);

  // Send projection matrix
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);


  glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
  glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

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
