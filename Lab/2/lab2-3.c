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

// Globals
// Data would normally be read from files
GLfloat myRotZ[] = {    1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotY[] = {    1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotX[] = {    1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat rotationMatrix[] = {	0.7f, 0.0f, -0.7f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.7f, 0.0f, 0.7f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };

GLfloat translationMatrix[] = {	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, -5.0f,
				0.0f, 0.0f, 0.0f, 1.0f };


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
  program = loadShaders("lab2-3.vert", "lab2-3.frag");
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

  myRotZ[0] = cos(t/1000);
  myRotZ[1] = -sin(t/1000);
  myRotZ[4] = sin(t/1000);
  myRotZ[5] = cos(t/1000);

  myRotY[0] = cos(t/5000);
  myRotY[2] = sin(t/5000);
  myRotY[8] = -sin(t/5000);
  myRotY[10] = cos(t/5000);

  myRotX[5] = cos(10.0);
  myRotX[6] = -sin(10.0);
  myRotX[9] = sin(10.0);
  myRotX[10] = cos(10.0);

  //  myTranX[3] = 5.0f;

  printError("pre display");

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Send Rotations to shaders
  //glUniformMatrix4fv(glGetUniformLocation(program, "myRotZ"), 1, GL_TRUE, myRotZ);
  glUniformMatrix4fv(glGetUniformLocation(program, "myRotY"), 1, GL_TRUE, myRotY);
  /* glUniformMatrix4fv(glGetUniformLocation(program, "myRotX"), 1, GL_TRUE, myRotX); */

  // Send Translation
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, rotationMatrix);
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, translationMatrix);

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
  glutCreateWindow ("Applying Texture To the White Rabbit");
  glutDisplayFunc(display);
  init ();
  glutTimerFunc(20, &OnTimer, 0);
  glutMainLoop();
}
