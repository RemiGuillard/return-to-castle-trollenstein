#version 150

in  vec3 inPos;
in  vec3 inNorm;

in	vec3 inTex;
out	vec3 oTex;

out vec3 oNorm;
//out vec3 fragPos;

// NY
uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
  mat4 worldView = projMatrix * mdlMatrix;
  gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPos, 1.0);
  
  oNorm = inNorm;//normalize(vec3(inNorm * mat4x3(worldView)));
  //fragPos = normalize(vec3(inPos * mat4x3(worldView)));
  
  oTex = inTex;
}
