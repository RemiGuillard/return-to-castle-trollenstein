#version 150

out	 vec4	out_Color;
in	 vec2	texCoord;
uniform	 sampler2D	texUnit;

void main(void)
{
	vec4 flower = texture(texUnit, texCoord.st);
	out_Color = flower;
}
