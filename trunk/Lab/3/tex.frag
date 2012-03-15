#version 150

out	 vec4	out_Color;
in	 vec3	exNorm;
in	 vec2	oTex;

uniform	 sampler2D	texUnit;

void main(void)
{
	const vec3 light = vec3(0.58,0.58,0.58);
	float shade;

	shade = dot(normalize(exNorm), light);

	vec4 myTex = texture(texUnit, oTex.st);
	out_Color = myTex * vec4(shade, shade, shade, 1.0);

}
