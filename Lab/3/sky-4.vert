#version 150

in	 vec3 inPos;
in	 vec2 inTex;

uniform mat4 projMatrix;
uniform mat4 camMatrix; // World to View
uniform mat4 mdlMatrix; // Model to World

out	 vec2 oTex;

void main(void)
{
	mat3 normalMatrix = mat3(camMatrix * mdlMatrix);

	gl_Position =  projMatrix * camMatrix * mdlMatrix * vec4(inPos, 1);

	oTex = inTex;
}
