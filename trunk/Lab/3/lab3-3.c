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
#include "LoadTGA.h"

// Globals
// Data would normally be read from files

#define M_PI 3.14409

//Projection matrix
#define near 1.0
#define far 100.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
				0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
				0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
				0.0f, 0.0f, -1.0f, 0.0f };


GLfloat ground[] = {

  -150.0f, 0.0f, -150.0f,
  -150.0f, 0.0f, 150.0f,
  150.0f, 0.0f, -150.0f,

  150.0f, 0.0f, 150.0f,
  -150.0f, 0.0f, 150.0f,
  150.0f, 0.0f, -150.0f,
};

// vertex array object
unsigned int bunnyVertexArrayObjID;

// Shaders 
GLuint program;
GLuint progsky;
GLuint progtex;

// Model
Model *walls;
Model *blade;
Model *balcony;
Model *roof;
Model *bunny;
Model *teddy;
Model *teapot;
Model *skybox;

//Texture
GLuint	flowerTex;
GLuint	teaTex;
GLuint	skyTex;

//Window Size
int height;
int width;

float angh;
float angv;

// Angle & pos matrix
GLfloat position[] =	{ 0.0f, 10.0f, 20.0f };
GLfloat look[] =	{ 0.0f, 10.0f, 0.0f };

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


void OnTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(20, &OnTimer, value);
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
    look[0] += 0.01 * AB.x;
    look[2] += 0.01 * AB.z;
    position[0] += 0.01 * AB.x;
    position[2] += 0.01 * AB.z;
  }
  if (keyIsDown('s')) { //DOWN
    SetVector(look[0] - position[0], look[1] - position[1], look[2] - position[2], &AB);
    look[0] -= 0.01 * AB.x;
    look[2] -= 0.01 * AB.z;
    position[0] -= 0.01 * AB.x;
    position[2] -= 0.01 * AB.z;
  }
  if (keyIsDown('d')) { //RIGHT
    rotationVectY(90, orig, dir);
    SetVector(dir[0] - orig[0], dir[1] - orig[1], dir[2] - orig[2], &AB);
    look[0] += 0.01 * AB.x;
    look[2] += 0.01 * AB.z;
    position[0] += 0.01 * AB.x;
    position[2] += 0.01 * AB.z;
  }
  if (keyIsDown('a') || keyIsDown('q')) { //LEFT
    rotationVectY(90, orig, dir);
    SetVector(dir[0] - orig[0], dir[1] - orig[1], dir[2] - orig[2], &AB);
    look[0] -= 0.01 * AB.x;
    look[2] -= 0.01 * AB.z;
    position[0] -= 0.01 * AB.x;
    position[2] -= 0.01 * AB.z;
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

void getWindowResize(int wid, int hei) {
  height = hei;
  width = wid;
  glViewport(0,0,width,height);
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
}

/// END LOOK AT //////
//////////////////////

unsigned int groundArrayObjID;

void initGround() {

  unsigned int groundBufferObjID;

  // Allocate and activate Vertex Array Object
  glGenVertexArrays(1, &groundArrayObjID);
  glBindVertexArray(groundArrayObjID);
  // Allocate Vertex Buffer Objects
  glGenBuffers(1, &groundBufferObjID);

  // VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, groundBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), ground, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "inPos"), 3 , GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "inPos"));

}

void init(void)
{
  // Reference to shader program
  dumpInfo();

  // GL inits
  glClearColor(0.2,0.2,0.5,0);
  glEnable(GL_DEPTH_TEST);
  printError("GL inits");

  angh = 0.0;
  angv = 0.0;

  // Load and compile shader
  program = loadShaders("lab3-3.vert", "lab3-3.frag");
  progsky = loadShaders("sky-3.vert", "sky-3.frag");
  progtex = loadShaders("tex-3.vert", "tex-3.frag");
  printError("init shader");

  //Load Texture
  LoadTGATextureSimple("maskros512.tga", &flowerTex);
  LoadTGATextureSimple("SkyBox512.tga", &skyTex);
  LoadTGATextureSimple("bilskissred.tga", &teaTex);
  printError("init Texture");  

  // Load object
  initGround();

  roof = LoadModelPlus("windmill/windmill-roof.obj", program, "inPos", "inNorm","inTex");
  printError("Init roof");

  walls = LoadModelPlus("windmill/windmill-walls.obj", program, "inPos", "inNorm","inTex");
  printError("Init walls");

  balcony = LoadModelPlus("windmill/windmill-balcony.obj", program, "inPos", "inNorm","inTex");
  printError("Init balcony");

  blade = LoadModelPlus("windmill/blade.obj", program, "inPos", "inNorm","inTex");
  printError("Init Blade");
  
  teddy = LoadModelPlus("teddy.obj", program, "inPos", "inNorm","inTex");
  printError("Init teddy");
  
  teapot = LoadModelPlus("teapot.obj", progtex, "inPos", "inNorm","inTex");
  printError("Init teapot");
  
  skybox = LoadModelPlus("skybox.obj", progsky, "inPos", "inNorm","inTex");
  printError("Init skybox");

  bunny = LoadModelPlus("bunnyplus.obj", progtex, "inPos", "inNorm","inTex");
  printError("Init bunny");
  
}

void display(void)
{
  float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

  GLfloat roofMatrix[16];
  GLfloat balconyMatrix[16];
  GLfloat wallsMatrix[16];
  GLfloat bladeMatrix[16];
  GLfloat bunnyMatrix[16];
  GLfloat teddyMatrix[16];
  GLfloat teapotMatrix[16];
  GLfloat skyboxMatrix[16];

  IdentityMatrix(roofMatrix);
  IdentityMatrix(balconyMatrix);
  IdentityMatrix(wallsMatrix);
  IdentityMatrix(bladeMatrix);
  IdentityMatrix(bunnyMatrix);
  IdentityMatrix(teddyMatrix);
  IdentityMatrix(teapotMatrix);
  IdentityMatrix(skyboxMatrix);

  moveThePlayer();

  GLfloat cam[16];
  lookAt(position[0], position[1], position[2],
	 look[0], look[1], look[2],
	 0.0, 1.0, 0.0,
	 cam);

  //Copy of CamMatrix without Translation for skybox
  GLfloat camBox[16];
  camBox[0] = cam[0]; camBox[1] = cam[1]; camBox[2] = cam[2]; camBox[3] = 0.0f;
  camBox[4] = cam[4]; camBox[5] = cam[5]; camBox[6] = cam[6]; camBox[7] = 0.0f;
  camBox[8] = cam[8]; camBox[9] = cam[9]; camBox[10] = cam[10]; camBox[11] = 0.0f;
  camBox[12] = cam[12]; camBox[13] = cam[13]; camBox[14] = cam[14]; camBox[15] = cam[15];

  printError("pre display");

  bladeMatrix[3] = 4.6;
  bladeMatrix[7] = 9.3;

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(progsky);

  // Bind CamMatrix for Skybox
  glUniformMatrix4fv(glGetUniformLocation(progsky, "camMatrix"), 1, GL_TRUE, camBox);
  // Send projection matrix
  glUniformMatrix4fv(glGetUniformLocation(progsky, "projMatrix"), 1, GL_TRUE, projectionMatrix);

  // SKYBOX => progsky
  glDisable(GL_DEPTH_TEST);
  glUniformMatrix4fv(glGetUniformLocation(progsky, "mdlMatrix"), 1, GL_TRUE, skyboxMatrix);
  glBindTexture(GL_TEXTURE_2D, skyTex);
  glUniform1i(glGetUniformLocation(progsky, "texUnit"), 0); // Texture unit 0
  DrawModel(skybox);
  glEnable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);

  // OBJECT WITHOUT TEXTURE => program  
  glUseProgram(program);
  // Bind Camera
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);

  // Bind for ground
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, skyboxMatrix);
  glBindVertexArray(groundArrayObjID);// Select VAO
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
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

  //Bind for teddy
  teddyMatrix[11] = -5.0;
  teddyMatrix[7] = 10.0;
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, teddyMatrix);
  DrawModel(teddy);
  

  // OBJECT WITH TEXTURE => progtex
  glUseProgram(progtex);

  glUniformMatrix4fv(glGetUniformLocation(progtex, "camMatrix"), 1, GL_TRUE, cam);
  glUniformMatrix4fv(glGetUniformLocation(progtex, "projMatrix"), 1, GL_TRUE, projectionMatrix);

  //bind for teapot
  teapotMatrix[11] = 10.0;
  teapotMatrix[7] = 10.0;
  teapotMatrix[4] = 10.0;
  glUniformMatrix4fv(glGetUniformLocation(progtex, "mdlMatrix"), 1, GL_TRUE, teapotMatrix);
  glBindTexture(GL_TEXTURE_2D, teaTex);
  glUniform1i(glGetUniformLocation(progtex, "inTex"), 0); // Texture unit 0 
  DrawModel(teapot);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  //Bind for bunny1
  bunnyMatrix[11] = -20.0;
  bunnyMatrix[7] = 3.0;
  glUniformMatrix4fv(glGetUniformLocation(progtex, "mdlMatrix"), 1, GL_TRUE, bunnyMatrix);
  if (bunny->texCoordArray != NULL) {
    glBindTexture(GL_TEXTURE_2D, flowerTex);
    glUniform1i(glGetUniformLocation(progtex, "texUnit"), 0); // Texture unit 0 
  }
  DrawModel(bunny);
  printError("Object Bunny1");
  //Disable last Texture
  glBindTexture(GL_TEXTURE_2D, 0);

  //Bind for bunny2
  bunnyMatrix[11] = 10.0;
  bunnyMatrix[7] = 5.0;
  glUniformMatrix4fv(glGetUniformLocation(progtex, "mdlMatrix"), 1, GL_TRUE, bunnyMatrix);
  if (bunny->texCoordArray != NULL) {
    glBindTexture(GL_TEXTURE_2D, flowerTex);
    glUniform1i(glGetUniformLocation(progtex, "texUnit"), 0); // Texture unit 0 
  }
  DrawModel(bunny);
  printError("Object bunny2");
  
  printError("display");
  glutSwapBuffers();
}


int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutCreateWindow ("Welcome to my Skybox");
  glutDisplayFunc(display);
  init ();
  glutPassiveMotionFunc(&mouseMove);
  initKeymapManager();
  glutReshapeFunc(&getWindowResize);
  glutTimerFunc(20, &OnTimer, 0);
  glutMainLoop();
}
