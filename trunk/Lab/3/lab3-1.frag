#version 150

out	 vec4	out_Color;

in	 vec3	exNorm;


uniform	 sampler2D	Texture;

void main(void)
{
	const vec3 light = vec3(0.58,0.58,0.58);
	float shade;

	shade = dot(normalize(exNorm), light);

	out_Color = vec4(shade, shade, shade, 1.0);
}
