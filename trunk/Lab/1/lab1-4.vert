#version 150

in  vec3 in_Position;
uniform mat4 myMatrix;
in	vec3 inColor;
out vec3 color;

void main(void)
{
	gl_Position = myMatrix * vec4(in_Position, 1.0);
	color = inColor;
}
