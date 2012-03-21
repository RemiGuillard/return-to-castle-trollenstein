// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include "GL_utilities.h"

// Globals
// Data would normally be read from files
GLfloat vertices[] = {	
  -0.5f,0.5f,0.5f,
  -0.5f,-0.5f,0.5f,
  0.5f,-0.5f,0.5f,

  -0.5f,0.5f,0.5f,
  0.5f,0.5f,0.5f,
  0.5f,-0.5f,0.5f,
  
  
  /*3*/	0.5f,0.5f,0.5f,
  0.5f,-0.5f,0.5f,
  0.5f,-0.5f,-0.5f,
  
  0.5f,0.5f,0.5f,
  0.5f,0.5f,-0.5f,
  0.5f,-0.5f,-0.5f,
  

  /*5*/	-0.5f,0.5f,0.5f,
  -0.5f,0.5f,-0.5f,
  -0.5f,-0.5f,-0.5f,
  
  -0.5f,0.5f,0.5f,
  -0.5f,-0.5f,0.5f,
  -0.5f,-0.5f,-0.5f,
  
  
  /*7*/	-0.5f,0.5f,-0.5f,
  -0.5f,-0.5f,-0.5f,
  0.5f,-0.5f,-0.5f,

  -0.5f,0.5f,-0.5f,
  0.5f,0.5f,-0.5f,
  0.5f,-0.5f,-0.5f,


  /*9*/   -0.5f,-0.5f,-0.5f,
  -0.5f,-0.5f,0.5f,
  0.5f,-0.5f,0.5f,
  
  -0.5f,-0.5f,-0.5f,
  0.5f,-0.5f,-0.5f,
  0.5f,-0.5f,0.5f,
  
  
  /*11*/	-0.5f,0.5f,-0.5f,
  -0.5f,0.5f,0.5f,
  0.5f,0.5f,0.5f,
  
  -0.5f,0.5f,-0.5f,
  0.5f,0.5f,-0.5f,
  0.5f,0.5f,0.5f


};

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




GLfloat color[] = {
    1.0f,1.0f,0.0f,
    1.0f,1.0f,0.0f,
    1.0f,1.0f,1.0f,

    1.0f,1.0f,0.0f,
    1.0f,1.0f,0.0f,
    1.0f,1.0f,1.0f,


    1.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,1.0f,

    1.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,1.0f,


    0.0f,1.0f,1.0f,
    0.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,

    0.0f,1.0f,1.0f,
    0.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,


    1.0f,0.0f,1.0f,
    1.0f,0.0f,1.0f,
    1.0f,1.0f,1.0f,

    1.0f,0.0f,1.0f,
    1.0f,0.0f,1.0f,
    1.0f,1.0f,1.0f,


    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,1.0f,

    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,1.0f,


    0.0f,0.0f,1.0f,
    1.0f,0.0f,1.0f,
    0.0f,1.0f,1.0f,

    0.0f,0.0f,1.0f,
    1.0f,0.0f,1.0f,
    0.0f,1.0f,1.0f

};

// vertex array object
unsigned int vertexArrayObjID;

// Shaders 
GLuint program;

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	// Reference to shader program


	dumpInfo();

	// GL inits
	glClearColor(0.2,0.7,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 108*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	
	// End of upload of geometry

	//Load Color
	// Allocate Color Buffer Objects
	glGenBuffers(1, &colorBufferObjID);
	
	// CBO for color data
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 108*sizeof(GLfloat), color, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inColor"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inColor"));

	
	printError("init arrays");
}


void display(void)
{
  float t = glutGet(GLUT_ELAPSED_TIME);

  myRotZ[0] = cos(t/1000);
  myRotZ[1] = -sin(t/1000);
  myRotZ[4] = sin(t/1000);
  myRotZ[5] = cos(t/1000);

  myRotY[0] = cos(t/2000);
  myRotY[2] = sin(t/2000);
  myRotY[8] = -sin(t/2000);
  myRotY[10] = cos(t/2000);

  myRotX[5] = cos(t/3000);
  myRotX[6] = -sin(t/3000);
  myRotX[9] = sin(t/3000);
  myRotX[10] = cos(t/3000);
  

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Give myRotation
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotZ"), 1, GL_TRUE, myRotZ);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotY"), 1, GL_TRUE, myRotY);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotX"), 1, GL_TRUE, myRotX);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36);	// draw object
	
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
