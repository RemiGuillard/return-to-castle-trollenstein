#version 150

out	 vec4	out_Color;
in	 vec3	exNormal;
in	 vec2	texCoord;

void main(void)
{
	const vec3 light = vec3(0.58,0.58,0.58);
	float shade;
	float a = sin(texCoord.s*30) / 2 + 0.5;
	float b = sin(texCoord.t*30) / 2 + 0.5;


	shade = dot(normalize(exNormal), light);
	out_Color = vec4(a*shade,b*shade,shade,0.0);
}
