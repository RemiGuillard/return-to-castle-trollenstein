#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

// Personnal includes
#include "Trollenstein.h"

//Window Size
int height;
int width;

//Terrain size
int tLength;
int tWidth;

//Angle pos
float angh;
float angv;

//MAP DATA
mapData *map;

// Angle & pos matrix
GLfloat position[] =	{ 0.0f, 0.5f, 0.0f };
GLfloat look[] =	{ 10.0f, 0.5f, 10.0f };
float move = 0.02;
// Are we flying or not ?!
int flying = 0;

// Object Model
Model *gsph;

// Texture
GLuint sphTex;

// Sphere coord obj
// Distance Matrix
#define near 0.1
#define far 500.0
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

void setPlayerAtStart() {
  int x, z;
  
  for (x = 0; x < map->width-1; x++)
	for (z = 0; z < map->height-1; z++)
	  if (map->map[z][x] == START) {
		position[0] = x + 0.5;
		position[2] = z + 0.5;
		look[0] = x + 0.5;
		look[2] = z + 2.5;
	  }
		
}

void moveThePlayer() {

  GLfloat orig[] = { position[0], position[1], position[2] };
  GLfloat dir[] = { look[0], look[1], look[2] };

  Point3D AB;

  GLfloat oldPos[] = { position[0], position[1], position[2] };
  GLfloat oldDir[] = { look[0], look[1], look[2] };
  
  if (keyIsDown(27))
    exit(EXIT_SUCCESS);

  if (keyIsDown('p')) {
	if (flying == 1) {
	  look[1] -= position[1];
	  position[1] = 0.5f;
	  flying = 0;
	}
  }

  if (keyIsDown('o'))
	  flying = 1;
  if (keyIsDown('z')) { //UP
    SetVector(look[0] - position[0], look[1] - position[1], look[2] - position[2], &AB);
    look[0] += move * AB.x;
    look[2] += move * AB.z;
    position[0] += move * AB.x;
    position[2] += move * AB.z;
	if (flying == 1) {
	  look[1] += move * AB.y;
	  position[1] += move * AB.y;
	}
  }
  if (keyIsDown('s')) { //DOWN
    SetVector(look[0] - position[0], look[1] - position[1], look[2] - position[2], &AB);
    look[0] -= move * AB.x;
    look[2] -= move * AB.z;
    position[0] -= move * AB.x;
    position[2] -= move * AB.z;
	if (flying == 1) {
	  look[1] -= move * AB.y;
	  position[1] -= move * AB.y;
	}
  }
  if (keyIsDown('d')) { //RIGHT
    rotationVectY(90, orig, dir);
    SetVector(dir[0] - orig[0], dir[1] - orig[1], dir[2] - orig[2], &AB);
    look[0] += move * AB.x;
    look[2] += move * AB.z;
    position[0] += move * AB.x;
    position[2] += move * AB.z;
	if (flying == 1) {
	  look[1] += move * AB.y;
	  position[1] += move * AB.y;
	}
  }
  if (keyIsDown('q')) { //LEFT
    rotationVectY(90, orig, dir);
    SetVector(dir[0] - orig[0], dir[1] - orig[1], dir[2] - orig[2], &AB);
    look[0] -= move * AB.x;
    look[2] -= move * AB.z;
    position[0] -= move * AB.x;
    position[2] -= move * AB.z;
	if (flying == 1) {
	  look[1] -= move * AB.y;
	  position[1] -= move * AB.y;
	}
  }
  if (keyIsDown('a')) { // Move camera left
	rotationVectY(-ANGMOV, position, look);
	angh -= ANGMOV;
	angh = fmodf(angh, 360.0);
  }
  if (keyIsDown('e')) { // Move camera right
	rotationVectY(ANGMOV, position, look);
	angh += ANGMOV;
	angh = fmodf(angh, 360.0);
  }
  if (keyIsDown('g')) { // Move camera down
	if (angv - ANGMOV > -179.0) {
	  if (look[2] > position[2])
		rotationVectX(-ANGMOV, position, look);
	  else
		rotationVectX(ANGMOV, position, look);
	  angv -= ANGMOV;
	}
  }
  if (keyIsDown('t')) { // move camera up
	if (angv + ANGMOV < 179.0) {
	  if (look[2] > position[2])
		rotationVectX(ANGMOV, position, look);
	  else
		rotationVectX(-ANGMOV, position, look);
	  angv += ANGMOV;
	}
  }
  if (keyIsDown('r')) { // restart the level (player at start)
	setPlayerAtStart();
  }

  if (position[0] < 0)
	position[0] = 0;
  if (position[0] > map->width)
	position[0] = map->width;
  if (position[2] < 0)
	position[2] = 0;
  if (position[2] > map->height)
	position[2] = map->height;
  if (map->map[(int)position[2]][(int)position[0]] == WALL) {
    look[0] = oldDir[0];
    look[2] = oldDir[2];
    position[0] = oldPos[0];
    position[2] = oldPos[2];

  }
}

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

Model* GenerateGround(mapData *map,
							GLuint program,
							char* vertexVariableName,
							char* normalVariableName,
							char* texCoordVariableName)
{
  // NEED TO ADD 1 to get gound number of vertices.
  // DON'T FORGET TO GET OFF AT THE END OF THE FUNCTION
  map->width += 1;
  map->height += 1;

  int vertexCount = map->width * map->height;
  int triangleCount = (map->width-1) * (map->height-1) * 2;
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

  for (x = 0; x < map->width; x++)
	for (z = 0; z < map->height; z++)
	  {

		// Vertex array
		model->vertexArray[(x + z * map->width)*3 + 0] = x / 1.0;
		model->vertexArray[(x + z * map->width)*3 + 1] = 0.0;
		model->vertexArray[(x + z * map->width)*3 + 2] = z / 1.0;
		// Normal vectors
		model->normalArray[(x + z * map->width)*3 + 0] = 0.0;
		model->normalArray[(x + z * map->width)*3 + 1] = 1.0;
		model->normalArray[(x + z * map->width)*3 + 2] = 0.0;
		// Texture coordinates
		model->texCoordArray[(x + z * map->width)*2 + 0] = x; // (float)x / map->width;
		model->texCoordArray[(x + z * map->width)*2 + 1] = z; // (float)z / map->height;
	  }

  for (x = 0; x < map->width-1; x++)
	for (z = 0; z < map->height-1; z++)
	  {
		// Triangle 1
		model->indexArray[(x + z * (map->width-1))*6 + 0] = x + z * map->width;
		model->indexArray[(x + z * (map->width-1))*6 + 1] = x + (z+1) * map->width;
		model->indexArray[(x + z * (map->width-1))*6 + 2] = x + 1 + z * map->width;
		// Triangle 2
		model->indexArray[(x + z * (map->width-1))*6 + 3] = x+1 + z * map->width;
		model->indexArray[(x + z * (map->width-1))*6 + 4] = x + (z+1) * map->width;
		model->indexArray[(x + z * (map->width-1))*6 + 5] = x+1 + (z+1) * map->width;
	  }

  map->width -= 1;
  map->height -= 1;


  // End of ground generation
  printError("Debug initialization of arrays");
  
  // Upload and set variables like LoadModelPlus
  glGenVertexArrays(1, &model->vao);
  glGenBuffers(1, &model->vb);
  glGenBuffers(1, &model->ib);
  glGenBuffers(1, &model->nb);
  if (model->texCoordArray != NULL)
	glGenBuffers(1, &model->tb);
  
  glBindVertexArray(model->vao);
  printError("Debug Bind Buffer");  

  // VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, model->vb);
  glBufferData(GL_ARRAY_BUFFER, model->numVertices*3*sizeof(GLfloat), model->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, vertexVariableName), 3, GL_FLOAT, GL_FALSE, 0, 0); 
  glEnableVertexAttribArray(glGetAttribLocation(program, vertexVariableName));
  printError("Debug init buffer Vertex Array");  
  
	// VBO for normal data
  glBindBuffer(GL_ARRAY_BUFFER, model->nb);
  glBufferData(GL_ARRAY_BUFFER, model->numVertices*3*sizeof(GLfloat), model->normalArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, normalVariableName), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, normalVariableName));
  printError("Debug init buffer Normal array");  
  
  // VBO for texture coordinate data
  if (model->texCoordArray != NULL)
	{
	  glBindBuffer(GL_ARRAY_BUFFER, model->tb);
	  glBufferData(GL_ARRAY_BUFFER, model->numVertices*2*sizeof(GLfloat), model->texCoordArray, GL_STATIC_DRAW);
	  glVertexAttribPointer(glGetAttribLocation(program, texCoordVariableName), 2, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(glGetAttribLocation(program, texCoordVariableName));
	}
  printError("Debug buffer Tex Array");  
	
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ib);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->numIndices*sizeof(GLuint), model->indexArray, GL_STATIC_DRAW);
  printError("Debug buffer Index array");  

  return model;
}

int		getNbWalls(mapData *map) {
  int x, z;
  int nbWalls = 0;
  for (x = 0; x < map->width; x++) {
	for (z = 0; z < map->height; z++) {
	  if (map->map[z][x] == WALL)
		nbWalls++;
	}
  }
  return nbWalls;
}

// vertex array object
Model *ground, *wall, *skybox;
// Reference to shader program
GLuint program, skyprog;
GLuint texGround, texWall, texSky;

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
	program = loadShaders("castle.vert", "castle.frag");
	skyprog = loadShaders("skybox.vert", "skybox.frag");
	glUseProgram(program);
	printError("Shader initialization");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("Textures/g_floor03.tga", &texGround);
	LoadTGATextureSimple("Textures/g_bricks30.tga", &texWall);
	LoadTGATextureSimple("Textures/SkyBox512.tga", &texSky);
	printError("Textures initialization");
	
	// Load terrain data

	map = loadMap("map");
	/* map = malloc(sizeof(map)); */
	/* map->width = 50; */
	/* map->height = 10; */
	/* map->map = malloc(sizeof(char*) * 10); */
	/* map->map[0] = "11111111111111111111111111111111111111111111111111"; */
	/* map->map[1] = "12000000000000000000011011101110000001000100000001"; */
	/* map->map[2] = "11111111011111101110000000000000110101010101101101"; */
	/* map->map[3] = "11000011110000010001111111111111110100010001101101"; */
	/* map->map[4] = "10000101111111000100000000000000000110111110000001"; */
	/* map->map[5] = "11110100000100011011110111101110111010110110111101"; */
	/* map->map[6] = "13000110110101000001011110000000000000000000111001"; */
	/* map->map[7] = "11101110010001101100000011110001101101100110010011"; */
	/* map->map[8] = "11000000001110001111101110000001110001110000000111"; */
	/* map->map[9] = "11111111111111111111111111111111111111111111111111"; */
	
	ground = GenerateGround(map, program, "inPos", "inNorm", "inTex");
	printError("Ground init");
	wall = LoadModelPlus("models/cube.obj", program, "inPos", "inNorm", "inTex");
	printError("Walls init");

	glUseProgram(skyprog);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	skybox = LoadModelPlus("models/skybox.obj", skyprog, "inPos", "inNorm", "inTex");
	printError("Skybox init");
	
	setPlayerAtStart();
	
	//	printError("Objects Initialisation");
}

void display(void)
{
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  GLfloat total[16], modelView[16], camMatrix[16];
  moveThePlayer();

  printError("pre display");
	

  // Build matrix
  lookAt(position[0], position[1], position[2],
		 look[0], look[1], look[2],
		 0, 1, 0,
		 camMatrix);
  GLfloat camBox[16] = { camMatrix[0],  camMatrix[1],  camMatrix[2],  0.0,
						 camMatrix[4],  camMatrix[5],  camMatrix[6],  0.0,
						 camMatrix[8],  camMatrix[9],  camMatrix[10], 0.0,
						 camMatrix[12], camMatrix[13], camMatrix[14], camMatrix[15]};
  
  IdentityMatrix(modelView);

  glUseProgram(skyprog);
  glDisable(GL_DEPTH_TEST);
  glUniformMatrix4fv(glGetUniformLocation(skyprog, "mdlMatrix"), 1, GL_TRUE, camBox);
  glBindTexture(GL_TEXTURE_2D, texSky);
  DrawModel(skybox);
  glEnable(GL_DEPTH_TEST);
  
  glUseProgram(program);
  Mult(camMatrix, modelView, total);

  
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
  glBindTexture(GL_TEXTURE_2D, texGround);		// Bind Our Texture tex1
  DrawModel(ground);
  
  int x, z;
  glBindTexture(GL_TEXTURE_2D, texWall);		// Bind Our Texture tex1
  for (x = 0; x < map->width; x++)
	for (z = 0; z < map->height; z++) {
	  if (map->map[z][x] == WALL) {
		modelView[3] = x;
		modelView[11] = z;
		Mult(camMatrix, modelView, total);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
		DrawModel(wall);
	  }
	}
  
  printError("Display Ground");
  
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
	glutCreateWindow ("Return to Castle of Trollenstein");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();

	//	glutPassiveMotionFunc(&mouseMove);
	glutReshapeFunc(&getWindowResize);
	glutTimerFunc(20, &timer, 0);
	
	glutMainLoop();
	exit(0);
}
