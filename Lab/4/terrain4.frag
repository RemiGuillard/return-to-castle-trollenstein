#version 150

in vec3 oNorm;
out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;

void main(void)
{

  const vec3 light = vec3(0.58,0.58,0.58);
  float shade;

  shade = dot(normalize(oNorm), light);
  outColor = texture(tex, texCoord) * vec4(shade, shade, shade, 1.0);
}
