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
out vec3 lightPos;
out vec3 lightWarp;

uniform sampler2D texNorm;

// MATRICES
uniform vec3 lightSourcePlayer;
uniform vec3 lightSourceWarp;
uniform mat4 projMatrix;
uniform mat4 camMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
  gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPos, 1);
   
  vec3 myNorm = vec3(2.0 * texture(texNorm, inTex.st) - 1.0);
  oNorm = normalize(vec3(mat3(camMatrix) * mat3(mdlMatrix)
						 * inNorm));
  
  fragPos = vec3(camMatrix * mdlMatrix * vec4(inPos,1.0));

  lightPos = vec3(camMatrix * vec4(lightSourcePlayer, 1.0));
  lightWarp = vec3(camMatrix * mdlMatrix * vec4(lightSourceWarp,1.0));
  oTex = inTex;
}
