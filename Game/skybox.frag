#version 150

out	 vec4	out_Color;
in	 vec2	oTex;
in   vec3   oNorm;

uniform	 sampler2D	tex;

void main(void)
{
	vec4 myTex = texture(tex, oTex.st);
	out_Color = myTex;// * vec4(oNorm, 1.0);
}
