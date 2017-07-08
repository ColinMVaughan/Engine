#version 420

uniform mat4 ViewToShadowMap;
uniform vec3 LightDirection;

//colour
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;

//scalars
uniform float LightSpecularExponent;


uniform sampler2D uScene;
uniform sampler2D uShadowMap;
uniform sampler2D uNormalMap;
uniform sampler2D uPositionMap;

in vec2 texcoord;

out vec4 outColor;

void main()
{
	///Get Data///
	vec3 textureColour = texture(uScene, texcoord).rgb;
	vec3 normal = texture(uNormalMap, texcoord).xyz * 2.0 - 1.0;
	vec3 pos = texture(uPositionMap, texcoord).xyz;

	if(length(pos) == 0.0)
	{
		discard;
	}


 outColor.rgb = LightAmbient;
 
 float NdotL = dot(normal, LightDirection);

 if (NdotL > 0.0)
 {
 //the light contributes to this surface
 
 //calculate Diffuse contribution
 outColor.rgb += LightDiffuse * NdotL;
 
 //Blinn-Phong half vector
 float NdotHV = max(dot(normal, normalize(LightDirection + normalize(-pos))), 0.0);
 
 //Calculate specular contribution
 outColor.rgb += LightSpecular * pow(NdotHV, LightSpecularExponent);
 }
 

 outColor.rgb *= textureColour.rgb;
 outColor.a = 1.0;
 
 
 ///Determine if shadowed.///
 vec4 shadowCoord = ViewToShadowMap * vec4(pos,1.0);

 //Where is the current point on the shadow map? What is the depth there?
 float shadowDepth = texture(uShadowMap, shadowCoord.xy).r;

 //Is there ar occluder there?
 if(shadowDepth <shadowCoord.z - 0.001f)
 {
	outColor.rgb *=0.4;
	//outColor.rgb *= 0;
 }

}