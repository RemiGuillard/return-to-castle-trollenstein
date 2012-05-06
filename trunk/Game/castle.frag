#version 150

in vec3 oNorm;
in vec3 oTex;
out vec4 outColor;

uniform sampler2D tex;

void main(void)
{

  const vec3 light = vec3(0.58,0.58,0.58);
  float shade;

  shade = dot(normalize(oNorm), light);
  vec4 myTex = texture(tex, oTex.st);
  outColor = myTex * vec4(shade, shade, shade, 1.0);
}
