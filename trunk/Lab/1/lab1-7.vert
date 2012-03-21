#version 150

in  vec3 inPosition;
in  vec3 inNormal;
uniform mat4 myRotZ;
uniform mat4 myRotY;
uniform mat4 myRotX;
out vec3 color;
out vec3 exNormal;

void main(void)
{
	gl_Position = myRotY * vec4(inPosition,
	1.0);
	mat3 normalMatrix = mat3(myRotZ);
	exNormal = inNormal * normalMatrix;
}
