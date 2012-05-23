#include "Display.h"

void display(void)
{
  GLfloat total[16], modelView[16], camMatrix[16];
  IdentityMatrix(modelView);
  IdentityMatrix(total);

  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Apply the changes of position
  // for the player with Keyboard events
  moveThePlayer();
  printError("Player movements");
	

  // Set the camera matrix
  lookAt(position[0], position[1], position[2],
		 look[0], look[1], look[2],
		 0, 1, 0,
		 camMatrix);

  // Set the camera matrix for the skybox
  GLfloat camBox[16] = { camMatrix[0],  camMatrix[1],  camMatrix[2],  0.0,
						 camMatrix[4],  camMatrix[5],  camMatrix[6],  0.0,
						 camMatrix[8],  camMatrix[9],  camMatrix[10], 0.0,
						 camMatrix[12], camMatrix[13], camMatrix[14], camMatrix[15]};
  

  // SET THE DISPLAY FOR THE SKYBOX
  glUseProgram(skyprog);
  glDisable(GL_DEPTH_TEST);
  glUniformMatrix4fv(glGetUniformLocation(skyprog, "mdlMatrix"), 1, GL_TRUE, modelView);
  glUniformMatrix4fv(glGetUniformLocation(skyprog, "camMatrix"), 1, GL_TRUE, camBox);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texSky);
  glUniform1i(glGetUniformLocation(skyprog, "tex"), 0); // Texunit 0
  DrawModel(skybox);
  glEnable(GL_DEPTH_TEST);

  // SET THE DISPLAY FOR THE MAP OBJECTS
  glUseProgram(program);
  // SET THE POSITION OF THE WARP
  modelView[3] = warpPos.x+0.5;
  modelView[7] = 0.5;
  modelView[11] = warpPos.z+0.5;
  modelView[0] = 0.05;
  modelView[5] = 0.05;
  modelView[10] = 0.05;
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);

  // BIND THE TEXTURE FOR THE WARP
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texWarp);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texWallN);
  glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texunit 0
  glUniform1i(glGetUniformLocation(program, "texNorm"), 1); // Texunit 1 

  GLfloat pos[] = { position[0] , 0.5, position[2]};
  glUniform3fv(glGetUniformLocation(program, "lightSourcePlayer"), 1, pos);
  pos[0] = warpPos.x + 0.5 ; pos[1] = 0.1; pos[2] = warpPos.z + 0.5;
  glUniform3fv(glGetUniformLocation(program, "lightSourceWarp"), 1, pos);

  // DRAW THE WARP SOME TIME SO THE WARP LOCATION IS EASIER TO FIND
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView);
  DrawModel(warp);

  // SEND MATRICES FOR THE DISPLAY
  IdentityMatrix(modelView);  
  glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);
  glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView);
  // BIND THE TEXTURE FOR THE GROUND
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texGround);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texGroundN);
  glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texunit 0
  glUniform1i(glGetUniformLocation(program, "texNorm"), 1); // Texunit 1 
  DrawModel(ground);

  // SET THE DISPLAY AND DRAWS THE WALLS
  int x, z;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texWall);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texWallN);
  glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texunit 0
  glUniform1i(glGetUniformLocation(program, "texNorm"), 1); // Texunit 1 

  modelView[7] = 0.0;
  for (x = 0; x < map->width; x++)
    for (z = 0; z < map->height; z++) {
      if (map->map[z][x] == WALL) {
		modelView[3] = x;
		modelView[11] = z;
		//		Mult(camMatrix, modelView, total);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView);
		DrawModel(wall);
      }
    }
  
  printError("Display Map");
  
  glutSwapBuffers();
}
