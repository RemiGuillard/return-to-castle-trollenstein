#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

// Personnal includes
#include "Trollenstein.h"
#include "MazeUtilities.h"
#include "Display.h"

//Window Size
int height;
int width;

//Angle pos
float angh;
float angv;

char** mapList;
int currentMap = 0;

//MAP DATA
mapData *map;

// Angle & pos matrix
GLfloat position[] =	{ 0.0f, 0.5f, 0.0f };
GLfloat look[] =	{ 10.0f, 0.5f, 10.0f };
Point3D warpPos = {0.0f,0.5f,0.0f};

// Are we flying or not ?!
int flying = 0;

// Distance Matrix
GLfloat projectionMatrix[] = {	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
								0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
								0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
								0.0f, 0.0f, -1.0f, 0.0f };


Point3D getWarpPosition() {
  int x, z;
  
  for (x = 0; x < map->width-1; x++)
	for (z = 0; z < map->height-1; z++)
	  if (map->map[z][x] == WARP) {
		warpPos.x = x;
		warpPos.y = 0;
		warpPos.z = z;
	  }	
  return warpPos;
}

void getWindowResize(int wid, int hei) {
  height = hei;
  width = wid;
  glViewport(0,0,width,height);
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

// vertex array object
Model *ground, *wall, *skybox, *warp;
// Reference to shader program
GLuint program, skyprog;
GLuint texGround, texWall, texGroundN, texWallN, texSky, texWarp;
GLuint tgro1, tgron1, twall1, twalln1;
GLuint tgro2, tgron2, twall2, twalln2;

void setTextures() {

  if (currentMap % 2 == 0) {
	texGround = tgro1;
	texWall = twall1;
	texGroundN = tgron1;
	texWallN = twalln1;
  } else if (currentMap % 2 == 1) {
	texGround = tgro2;
	texWall = twall2;
	texGroundN = tgron2;
	texWallN = twalln2;
  }
}

void changeMap() {

  // Load terrain data
  
  if (map != NULL) {
    int i = 0;
    while (i < map->height) {
      free(map->map[i]);
      i++;
    }
    free(map->map);
    free(map);
  }
  map = loadMap(mapList[currentMap++]);
  if (map == NULL) {
    printf("Congrats, you've find the way to the exit\n");
    exit(0);
  }
  printError("Load Map init");
  printf("Find the way to the exit of the level\n");

  if (ground != NULL) {
    free(ground->vertexArray);
    free(ground->normalArray);
    free(ground->texCoordArray);
    free(ground->indexArray);
    free(ground);
  }
  ground = GenerateGround(map, program, "inPos", "inNorm", "inTex");
  printError("Ground init");

  setTextures();
  
  setPlayerAtStart();
  getWarpPosition();
}
 
void init(void)
{
  // GL Init components
  glClearColor(0.2,0.2,0.5,0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  printError("GL inits");

  // INITIALISATION OF ANGLES
  angh = 0.0;
  angv = 0.0;
	
  // Load and compile shaders for the map and skybox
  program = loadShaders("shaders/castle.vert", "shaders/castle.frag");
  skyprog = loadShaders("shaders/skybox.vert", "shaders/skybox.frag");
  printError("Shader initialization");

  // LOAD THE OBJECTS WALLS AND WARP
  // Here Warp is a wall, we didn't find a good object
  // To which one we could apply psychedelic textures to
  // Really simulates a warp (By example, moving with a rotation
  // depending of the time)...
  glUseProgram(program);
  wall = LoadModelPlus("models/cube.obj", program, "inPos", "inNorm", "inTex");

  // SEND DATA TO SHADERS
  glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
  //  glUniform1i(glGetUniformLocation(program, "tex"), 0);

  // TEXTURES LOADING
  LoadTGATextureSimple("Textures/Warp.tga", &texWarp);

  LoadTGATextureSimple("Textures/stonewallDiffuse.tga", &twall1);
  LoadTGATextureSimple("Textures/stonewallNormal.tga", &twalln1);
  LoadTGATextureSimple("Textures/metalSheetDiffuse.tga", &tgro1);
  LoadTGATextureSimple("Textures/metalSheetNormal.tga", &tgron1);

  LoadTGATextureSimple("Textures/bricksDiffuse.tga", &twall2);
  LoadTGATextureSimple("Textures/bricksNormal.tga", &twalln2);
  LoadTGATextureSimple("Textures/cobblestonesDiffuse.tga", &tgro2);
  LoadTGATextureSimple("Textures/cobblestonesNormal.tga", &tgron2);

  warp = LoadModelPlus("models/teapot.obj", program, "inPos", "inNorm", "inTex");
  
  printError("Textures initialization");
  
  // SKYBOX LOADING
  glUseProgram(skyprog);

  LoadTGATextureSimple("Textures/SkyBox512.tga", &texSky);

  glUniformMatrix4fv(glGetUniformLocation(skyprog, "projMatrix"), 1, GL_TRUE, projectionMatrix);
  skybox = LoadModelPlus("models/skybox.obj", skyprog, "inPos", "inNorm", "inTex");

  
  printError("Skybox init");

  // Change map function
  changeMap();

}

void timer(int i)
{
  glutTimerFunc(20, &timer, i);
  glutPostRedisplay();
}
 
void getMapList(char **list) {
  mapList = list;
}
 
int main(int argc, char **argv)
{
  if (argc < 2)
    return 0;
  getMapList(&argv[1]);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize (1000, 600);
  glutCreateWindow ("Return to Castle of Trollenstein");
  glutDisplayFunc(display);
  init ();
  initKeymapManager();

  //	glutPassiveMotionFunc(&mouseMove);
  glutReshapeFunc(&getWindowResize);
  glutTimerFunc(20, &timer, 0);
  
  glutMainLoop();
  return 0;
}
