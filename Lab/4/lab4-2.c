
// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGLUT.h"
	// A little linking trick for Lightweight IDE
	#if 0
		#import <Cocoa/Cocoa.h>
	#endif
#endif
#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

// Personnal includes

//Window Size
int height;
int width;

//Angle pos
float angh;
float angv;

// Angle & pos matrix
GLfloat position[] =	{ 0.0f, 10.0f, 20.0f };
GLfloat look[] =	{ 15.0f, 0.0f, 0.0f };

// Are we flying or not ?!
int flying = 1;

#define near 0.2
#define far 50.0
#define right 0.1
#define left -0.1
#define top 0.1
#define bottom -0.1
GLfloat projectionMatrix[] = {	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
											0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
											0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
											0.0f, 0.0f, -1.0f, 0.0f };


// Function for movement
#define M_PI 3.14409

void getWindowResize(int wid, int hei) {
  height = hei;
  width = wid;
  glViewport(0,0,width,height);
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
  xprim = AB.y * cos(getRad(angle)) - AB.z * sin(getRad(angle));
  yprim = AB.y * sin(getRad(angle)) + AB.z * cos(getRad(angle));

  B[1] = xprim + A[1];
  B[2] = yprim + A[2];
}


void moveThePlayer() {

  GLfloat orig[] = { position[0], position[1], position[2] };
  GLfloat dir[] = { look[0], look[1], look[2] };

  Point3D AB;

  if (keyIsDown(27))
    exit(EXIT_SUCCESS);

  if (keyIsDown('w') || keyIsDown('z')) { //UP
    SetVector(look[0] - position[0], look[1] - position[1], look[2] - position[2], &AB);
    look[0] += 0.04 * AB.x;
    look[2] += 0.04 * AB.z;
    position[0] += 0.04 * AB.x;
    position[2] += 0.04 * AB.z;
	if (flying == 1) {
	  look[1] += 0.04 * AB.y;
	  position[1] += 0.04 * AB.y;
	}
  }
  if (keyIsDown('s')) { //DOWN
    SetVector(look[0] - position[0], look[1] - position[1], look[2] - position[2], &AB);
    look[0] -= 0.04 * AB.x;
    look[2] -= 0.04 * AB.z;
    position[0] -= 0.04 * AB.x;
    position[2] -= 0.04 * AB.z;
	if (flying == 1) {
	  look[1] -= 0.04 * AB.y;
	  position[1] -= 0.04 * AB.y;
	}
  }
  if (keyIsDown('d')) { //RIGHT
    rotationVectY(90, orig, dir);
    SetVector(dir[0] - orig[0], dir[1] - orig[1], dir[2] - orig[2], &AB);
    look[0] += 0.04 * AB.x;
    look[2] += 0.04 * AB.z;
    position[0] += 0.04 * AB.x;
    position[2] += 0.04 * AB.z;
	if (flying == 1) {
	  look[1] += 0.04 * AB.y;
	  position[1] += 0.04 * AB.y;
	}
  }
  if (keyIsDown('a') || keyIsDown('q')) { //LEFT
    rotationVectY(90, orig, dir);
    SetVector(dir[0] - orig[0], dir[1] - orig[1], dir[2] - orig[2], &AB);
    look[0] -= 0.04 * AB.x;
    look[2] -= 0.04 * AB.z;
    position[0] -= 0.04 * AB.x;
    position[2] -= 0.04 * AB.z;
	if (flying == 1) {
	  look[1] -= 0.04 * AB.y;
	  position[1] -= 0.04 * AB.y;
	}
  }
}

void mouseMove(int x, int y) {
  float factx = ((float)(x) - (float)(width/2)) / (float)(width/2);
  float facty = ((float)(-y) + (float)(height/2)) / (float)(width/2);
  float angleh, anglev;

  //Horizontal
  angleh = (float)factx * 360.0;
  rotationVectY(angleh - angh, position, look);
  angh = angleh;

  //Vertical
  
  anglev = (float)facty * 179.0;
  if (look[2] > position[2])
	rotationVectX(-anglev + angv, position, look);
  else
	rotationVectX(anglev - angv, position, look);
  angv = anglev;

}


// This they should have done themselves in lab 2
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
//	Normalize(&v); // Should be unnecessary
	
	T(-px, -py, -pz, t);
	
	r[0] = u.x; r[1] = u.y; r[2] = u.z;  r[3] = 0;
	r[4] = v.x; r[5] = v.y; r[6] = v.z;  r[7] = 0;
	r[8] = n.x; r[9] = n.y; r[10] = n.z; r[11] = 0;
	r[12] = 0;   r[13] = 0;   r[14] = 0;   r[15] = 1;
	
	Mult(r, t, m);
}


Model* GenerateTerrain(TextureData *tex,
							GLuint program,
							char* vertexVariableName,
							char* normalVariableName,
							char* texCoordVariableName)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	Model* model = malloc(sizeof(Model));
	memset(model, 0, sizeof(Model));
	
	model->vertexArray = malloc(sizeof(Model));
	
	model->vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	model->normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	model->texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	model->indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	model->numVertices = vertexCount;
	model->numIndices = triangleCount*3;
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			model->vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			model->vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			model->vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			model->normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			model->normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			model->normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			model->texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			model->texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			model->indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			model->indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			model->indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			model->indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			model->indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			model->indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Upload and set variables like LoadModelPlus
	glGenVertexArrays(1, &model->vao);
	glGenBuffers(1, &model->vb);
	glGenBuffers(1, &model->ib);
	glGenBuffers(1, &model->nb);
	if (model->texCoordArray != NULL)
		glGenBuffers(1, &model->tb);
	
	glBindVertexArray(model->vao);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, model->vb);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices*3*sizeof(GLfloat), model->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, vertexVariableName), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, vertexVariableName));

	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, model->nb);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices*3*sizeof(GLfloat), model->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, normalVariableName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, normalVariableName));

	// VBO for texture coordinate data
	if (model->texCoordArray != NULL)
	{
		glBindBuffer(GL_ARRAY_BUFFER, model->tb);
		glBufferData(GL_ARRAY_BUFFER, model->numVertices*2*sizeof(GLfloat), model->texCoordArray, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, texCoordVariableName), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, texCoordVariableName));
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->numIndices*sizeof(GLuint), model->indexArray, GL_STATIC_DRAW);
	
	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	angh = 0.0;
	angv = 0.0;
	
	// Load and compile shader
	program = loadShaders("terrain2.vert", "terrain2.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	
// Load terrain data
	
	LoadTGATexture("ok1.tga", &ttex);
	//	LoadTGATexture("44-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex, program, "inPosition", "inNormal", "inTexCoord");
	printError("init terrain");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat total[16], modelView[16], camMatrix[16];
	moveThePlayer();
	
	printError("pre display");
	
	glUseProgram(program);

	// Build matrix
	lookAt(position[0], position[1], position[2],
		   look[0], look[1], look[2],
		   0, 1, 0,
		   camMatrix);
	IdentityMatrix(modelView);
	Mult(camMatrix, modelView, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm);

	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();

	glutPassiveMotionFunc(&mouseMove);
	glutReshapeFunc(&getWindowResize);
	glutTimerFunc(20, &timer, 0);
	
	glutMainLoop();
	exit(0);
}
