#version 420

uniform vec4 LightPosition;

//colour
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;

//scalars
uniform float LightSpecularExponent;
uniform float Attenuation_Constant;
uniform float Attenuation_Linear;
uniform float Attenuation_Quadratic;

uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

void main()
{
 outColor.rgb = LightAmbient;
 
 //account for rasterizer interpolating
 vec3 normal = normalize(norm);
 
 vec3 lightVec = LightPosition.xyz - pos;
 float dist = length(lightVec);
 vec3 lightDir = lightVec/dist;
 
 float NdotL = dot(normal, lightDir);

 if (NdotL > 0.0)
 {
 //the light contributes to this surface
 
 //Calculate attenuation (falloff)
 float attenuation = 1.0 / (Attenuation_Constant + (Attenuation_Linear * dist) + (Attenuation_Quadratic  * dist * dist));
 
 //calculate Diffuse contribution
 outColor.rgb += LightDiffuse * NdotL * attenuation;
 
 //Blinn-Phong half vector
 float NdotHV = max(dot(normal, normalize(lightDir + normalize(-pos))), 0.0);
 
 //Calculate specular contribution
 outColor.rgb += LightSpecular * pow(NdotHV, LightSpecularExponent) * attenuation;
 }
 
 vec4 textureColour = texture(uTex, texcoord);
 outColor.rgb *= textureColour.rgb;


 outColor.a = 0.5;
 
 
}