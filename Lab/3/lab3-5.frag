#version 150

out	 vec4	out_Color;

in	 vec3	Norm;
in	 vec3	fragPos;

uniform  vec3		lightSourcesDirPosArr[4];
uniform  vec3		lightSourcesColorArr[4];
uniform  float		specExponent[4];
uniform  bool		isDirectional[4];

vec3	calcDiffLight(vec3 lightDir, vec3 lightColor, vec3 normal, float lightIntensity) {

  vec3	diffLight;
  float	intensity;

  intensity = dot(lightDir, normal);
  intensity = max(intensity, 0.0);
  diffLight = clamp(lightColor * lightIntensity * intensity, 0.0, 1.0);
  
  return diffLight;
}

vec3	calcSpecLight(vec3 lightDir, vec3 normal, float speculat, float lightIntensity, vec3 viewDir, vec3 k) {

  vec3	specLight;
  vec3	refl;
  float	anglePhi;
  
  refl = normalize(2.0 * dot(normal, lightDir) * normal - lightDir);
  anglePhi = dot(refl, -viewDir);
  specLight = k * lightIntensity * max(0, pow(anglePhi, speculat));
  
  return specLight;
}

void main(void)
{
  int i = 0;

  float	ia = 0.75;
  float il = 0.75;
  vec3	k = vec3(0.60, 0.60, 0.60);
  
  vec3	ambDiff = k * ia, iDiff, iSpec;
  vec3	intensity, lightDir;
  
  intensity = ambDiff;
  iDiff = vec3(0.0, 0.0, 0.0);

  while (i < 4) {
	
	lightDir = normalize(lightSourcesDirPosArr[i] - vec3(fragPos));
	// CALC iDIFF
	iDiff = calcDiffLight(lightDir, lightSourcesColorArr[i], Norm, il);

	// CALC iSPEC
	iSpec = calcSpecLight(lightDir, Norm, specExponent[i], il, fragPos, k);
	
	// RESULT
	intensity += iDiff + iSpec;
	i++;
  }
  out_Color = vec4(intensity, 1.0);
}
