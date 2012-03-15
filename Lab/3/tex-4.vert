#version 150

in	 vec3 inPos;
in	 vec3 inNorm;

uniform mat4 projMatrix;
uniform mat4 camMatrix; // World to View
uniform mat4 mdlMatrix; // Model to World

in	 vec2 inTex;
out	 vec2 oTex;

out	 vec3 Norm;
out	 vec3 fragPos;

void main(void)
{
  
  mat4 worldView = projMatrix * mdlMatrix;
  
  gl_Position =  projMatrix * camMatrix * mdlMatrix * vec4(inPos, 1);

  Norm = normalize(vec3(inNorm * mat4x3(worldView)));
  fragPos = normalize(vec3(inPos * mat4x3(worldView)));

  oTex = inTex;
}
