#version 150

in	 vec3	oNorm;
in	 vec3	fragPos;

//uniform  vec3		lightSourceWarp;
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
  /*  float	ia = 0.75;
  float il = 0.75;
  vec3	k = vec3(0.05, 0.05, 0.05);
  
  vec3	ambDiff = k * ia;
  vec3	intensity, lightDir, iDiff, iSpec;

  intensity = ambDiff;
  iDiff = vec3(0.0, 0.0, 0.0);
  iSpec = vec3(0.0, 0.0, 0.0);

  lightDir = normalize(lightSourcePlayer - fragPos);
  //iDiff = calcDiffLight(lightDir, vec3(0.0,0.0,1.0), oNorm, il);
  iSpec = calcSpecLight(lightDir, oNorm, 60.0, il, fragPos, k);	
  intensity += iDiff + iSpec;
*/
  /*  lightDir = normalize(lightSourceWarp - fragPos);
      iDiff = calcDiffLight(lightDir, vec3(0.0,0.0,1.0), oNorm, il);
      iSpec = calcSpecLight(lightDir, oNorm, 30.0, il, fragPos, k);	
      intensity += iDiff + iSpec;*/
  //  float shade = dot(normalize(vec3()), normalize(oNorm));
  float shade = 0.75;
  vec4 myTex = texture(tex, oTex.st);
  outColor = myTex * vec4(shade, shade, shade, 1.0);// * vec4(intensity, 1.0);// * vec4(lightSourcePlayer, 1.0);
}

