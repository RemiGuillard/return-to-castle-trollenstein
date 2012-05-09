#include "Trollenstein.h"

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
