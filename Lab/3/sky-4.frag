#version 150

out	 vec4	out_Color;
in	 vec2	oTex;

uniform	 sampler2D	texUnit;

void main(void)
{
	
	vec4 myTex = texture(texUnit, oTex.st);
	out_Color = myTex;
}
