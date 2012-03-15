#version 150

in	 vec3 inPosition;
in  	 vec3 inNormal;
out 	 vec3 exNormal;
uniform mat4 myRotZ;
uniform mat4 myRotY;
uniform mat4 myRotX;
in	 vec2 inTexCoord;
out 	 vec2 texCoord;

void main(void)
{
	mat3 normalMatrix = mat3(myRotZ);
	exNormal = inNormal * normalMatrix;

	gl_Position =  myRotY * vec4(inPosition, 1);
	texCoord = inTexCoord;
}
