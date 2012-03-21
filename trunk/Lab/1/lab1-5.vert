#version 150

in  vec3 in_Position;
uniform mat4 myRotZ;
uniform mat4 myRotY;
uniform mat4 myRotX;
in	vec3 inColor;
out vec3 color;

void main(void)
{
	gl_Position = myRotZ * myRotY * myRotX * vec4(in_Position, 1.0);
	color = inColor;
}
