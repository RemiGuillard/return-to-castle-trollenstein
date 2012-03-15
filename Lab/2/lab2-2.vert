#version 150

in	 vec3 inPosition;
uniform mat4 myRotZ;
uniform mat4 myRotY;
uniform mat4 myRotX;
in	 vec2 inTexCoord;
out 	 vec2 texCoord;

void main(void)
{
	mat3 normalMatrix = mat3(myRotZ);

	gl_Position =  myRotY * vec4(inPosition, 1);
	texCoord = inTexCoord;
}
