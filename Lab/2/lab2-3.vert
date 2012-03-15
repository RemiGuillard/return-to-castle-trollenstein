#version 150

in	 vec3 inPosition;
in	 vec2 inTexCoord;
out 	 vec2 texCoord;
in	 vec3 inNormal;
out	 vec3 exNormal;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform mat4 myRotY;

void main(void)
{
	mat3 normalMatrix = mat3(camMatrix * myRotY);
//	mat3 normalMatrix = mat3(camMatrix * mdlMatrix);
	exNormal = normalMatrix * inNormal;

	gl_Position =  projMatrix * camMatrix * myRotY * vec4(inPosition, 1);
//	gl_Position =  projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1);
	texCoord = inTexCoord;
}
