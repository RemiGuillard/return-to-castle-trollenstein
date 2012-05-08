#version 150

out	 vec4	out_Color;
in	 vec2	oTex;

uniform	 sampler2D	tex;

void main(void)
{
	vec4 myTex = texture(tex, oTex.st);
	out_Color = myTex;
}
