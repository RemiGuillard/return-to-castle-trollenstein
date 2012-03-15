#version 150

in	 vec3 inPos;
in	 vec2 inTex;
in	 vec3 inNorm;

uniform mat4 projMatrix;
uniform mat4 camMatrix; // World to View
uniform mat4 mdlMatrix; // Model to World

out	 vec3 exNorm;

void main(void)
{
	mat3 normalMatrix = mat3(camMatrix * mdlMatrix);

	exNorm = normalMatrix * inNorm;

	gl_Position =  projMatrix * camMatrix * mdlMatrix * vec4(inPos, 1);

}
