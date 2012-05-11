#version 150

// POS & NORMAL
in  vec3 inPos;
in  vec3 inNorm;

// TEXTURE
in	vec3 inTex;
out	vec3 oTex;

// FRAGMENT VARIABLES
out vec3 oNorm;
out vec3 fragPos;

// MATRICES
uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
  mat4 worldView = projMatrix * mdlMatrix;
  gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPos, 1);
  
  oNorm = normalize(vec3(inNorm * mat4x3(worldView)));
  // fragPos = normalize(vec3(inPos * mat4x3(worldView)));
  fragPos = vec3(mdlMatrix * vec4(inPos,1.0));
  
  oTex = inTex;
}
