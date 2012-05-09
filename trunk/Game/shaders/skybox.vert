#version 150

in	 vec3 inPos;
in	 vec2 inTex;
in	 vec3 inNorm;

uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform mat4 mdlMatrix; // Model to World

out	 vec2 oTex;

void main(void)
{
  gl_Position =  projMatrix * camMatrix * vec4(inPos, 1);

  oTex = inTex;
}
