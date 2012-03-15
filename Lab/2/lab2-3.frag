#version 150

out	 vec4	out_Color;
in	 vec3	exNormal;
in	 vec2	texCoord;
uniform	 sampler2D	texUnit;

void main(void)
{
	const vec3 light = vec3(0.58,0.58,0.58);
	float shade;

	shade = dot(normalize(exNormal), light);
	vec4 flower = texture(texUnit, texCoord.st);
	out_Color = flower * vec4(shade, shade, shade, 1.0);
}
