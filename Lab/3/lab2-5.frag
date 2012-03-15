#version 150

out	 vec4	out_Color;

in	 vec3	exNorm;

in	 vec2	texCoord;

uniform	 sampler2D	Texture;

void main(void)
{
	const vec3 light = vec3(0.58,0.58,0.58);
	float shade;

	shade = dot(normalize(exNorm), light);
	vec4 tex = texture(Texture, texCoord.st);

	out_Color = tex * vec4(shade, shade, shade, 1.0);
}
