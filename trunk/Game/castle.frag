#version 150

in	 vec3	oNorm;
in	 vec3	fragPos;

uniform  vec3		lightSourceWarp;
uniform  vec3		lightSourcePlayer;

in vec3 oTex;
out vec4 outColor;

uniform sampler2D tex;


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
  float	ia = 0.75;
  float il = 0.75;
  vec3	k = vec3(0.05, 0.05, 0.05);
  
  vec3	ambDiff = k * ia;
  vec3	intensity, lightDir, iDiff, iSpec;

  intensity = ambDiff;
  iDiff = vec3(0.0, 0.0, 0.0);
  
  lightDir = normalize(lightSourceWarp - fragPos);
  // CALC iDIFF
  iDiff = calcDiffLight(lightDir, vec3(0.75,0.25,0.5), oNorm, il);
  // CALC iSPEC
  //  iSpec = calcSpecLight(lightDir, oNorm, 0.75, il, fragPos, k);	
  // RESULT
  intensity += iDiff;// + iSpec;

  vec4 myTex = texture(tex, oTex.st);
  outColor = myTex * vec4(intensity, 1.0);
}

