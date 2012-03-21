#version 150

in  vec3 inPosition;
in  vec3 inNormal;
uniform mat4 myRotZ;
uniform mat4 myRotY;
uniform mat4 myRotX;
out vec3 color;

void main(void)
{
	const vec3 light = vec3(0.6,0.6,0.6);
	float shade;

	shade = dot(normalize(inNormal), light);
	color = clamp(vec3(shade), 0.1, 0.95);

	gl_Position = myRotZ * myRotY * myRotX * vec4(inPosition, 1.0);
}
