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

  unsigned int bunnyVertexBufferObjID;
  unsigned int bunnyIndexBufferObjID;
  unsigned int bunnyNormalBufferObjID;


	// Reference to shader program
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.7,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	m = LoadModel("bunny.obj");

	// Load and compile shader
	program = loadShaders("lab1-7.vert", "lab1-7.frag");
	printError("init shader");

	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glGenBuffers(1, &bunnyVertexBufferObjID);
	glGenBuffers(1, &bunnyIndexBufferObjID);
	glGenBuffers(1, &bunnyNormalBufferObjID);
	
	glBindVertexArray(bunnyVertexArrayObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
	
	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
    
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
  

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Give myRotation
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotZ"), 1, GL_TRUE, myRotZ);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotY"), 1, GL_TRUE, myRotY);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotX"), 1, GL_TRUE, myRotX);

	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, const char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
}
