#version 150

// NORMAL AND FRAGMENT POSITION
in	 vec3	oNorm;
in	 vec3	fragPos;

// POSITION OF LIGHT SOURCE
uniform  vec3		lightSourcePlayer;
uniform  vec3		lightSourceWarp;

// TEXTURES
in vec3 oTex;
uniform sampler2D texUnit;
uniform sampler2D texNorm;

// OUTPUT
out vec4 outColor;

// CALCUL FOR DIFFUSE REFLECTION
vec3	calcDiffLight(vec3 lightDir, vec3 lightColor, vec3 normal, float lightIntensity) {

  vec3	diffLight;
  float	intensity;
  float lamberFact;

  intensity = dot(lightDir, normal);
  intensity = max(intensity, 0.0);
  lamberFact = max(dot(lightDir, normal), 0.0);
  diffLight = clamp(lightColor * lightIntensity * /*intensity*/lamberFact, 0.0, 1.0);
  
  return diffLight;
}

// CALCUL FOR SPECULAR LIGHT
vec3	calcSpecLight(vec3 lightDir, vec3 normal, float speculat, float lightIntensity, vec3 viewDir, vec3 k) {

  vec3	specLight;
  vec3	refl;
  float	anglePhi;
  float shininess;
  vec3 halfVector = normalize(fragPos + lightDir);
  refl = normalize(2.0 * dot(normal, lightDir) * normal - lightDir);
  anglePhi = dot(refl, -viewDir);
  //  specLight = k * lightIntensity * max(0, pow(anglePhi, speculat));
  specLight = k * lightIntensity * pow (max (dot (lightDir, normal), 0.0), 2.0);
  return specLight;
}

void main(void)
{
  float	ia = 0.05;
  float il = 0.5;
  vec3	k = vec3(0.5, 0.5, 0.5);
  
  vec3	ambDiff = k * ia;
  vec3	intensity, lightDir, iDiff, iSpec;

  intensity = ambDiff;
  iDiff = vec3(0.0, 0.0, 0.0);
  iSpec = vec3(0.0, 0.0, 0.0);

  vec3 myNorm = normalize(vec3(2.0 * texture(texNorm, oTex.st) - 1.0));

  lightDir = normalize(lightSourcePlayer - fragPos);
  iDiff = calcDiffLight(lightDir, vec3(0.80,0.70,0.70), myNorm, il);
  iSpec = calcSpecLight(lightDir, myNorm, 0.1, il, fragPos, k);	
  intensity += iDiff + iSpec;

  lightDir = normalize(lightSourceWarp - fragPos);
  iDiff = calcDiffLight(lightDir, vec3(0.80,0.0, 0.0), myNorm, il);
  iSpec = calcSpecLight(lightDir, myNorm, 0.1, il, fragPos, k);	
  intensity += iDiff + iSpec;

  vec4 myTex = texture(texUnit, oTex.st);
  outColor = myTex * vec4(intensity, 1.0);
}

