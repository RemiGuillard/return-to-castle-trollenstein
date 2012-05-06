#version 150

in  vec3 inPos;
in  vec3 inNorm;
in	vec3 inTex;
out	vec3 oTex;
out vec3 oNorm;
// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
  //  mat3 normalMatrix1 = mat3(mdlMatrix);
  gl_Position = projMatrix * mdlMatrix * vec4(inPos, 1.0);
  oNorm = /*normalMatrix1 **/ inNorm;
  oTex = inTex;
}
