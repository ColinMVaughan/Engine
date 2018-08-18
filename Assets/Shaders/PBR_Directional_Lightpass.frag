#version 420

//----------------------------------------
// Calculates the lighting for a sing point light
//-----------------------------------------

in vec2 texcoord;
out vec4 FragColor;

uniform vec3 camPos;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D positionMap;

uniform sampler2D materialMap;

uniform sampler2D shadowMap;

//lights
uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform mat4 lightSpaceMatrix;

const float PI = 3.14159265359;


//Cook-Torrance specular BRDF:
//DFG / (4(w0 . N)(wi . N))

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a		= roughness*roughness;
	float a2	= a*a;
	float NdotH	= max(dot(N,H),0.0);
	float NdotH2= NdotH *NdotH;
	
	float nom	= a2;
	float denom	= (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	
	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r 	= (roughness + 1.0);
	float k 	= (r*r) / 8.0;
	
	float nom 	= NdotV;
	float denom = NdotV * (1.0 - k) + k;
	
	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L), 0.0);
	float ggx2   = GeometrySchlickGGX(NdotV, roughness);
	float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	  return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}  

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; // not nessisary
	projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0)
	{
		float shadow = 0.0;
		return shadow;
	}

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
	return shadow;
}

//=================================================================================================
//=================================================================================================

void main()
{
	//------------------------------------------------------------
	//			Collect Data from Textures
	//------------------------------------------------------------

	//convert sRGB to linear color-space
	vec3 albedo	= texture(albedoMap, texcoord).rgb;
	albedo.x 	= pow(albedo.x, 2.2);
	albedo.y	= pow(albedo.y, 2.2);
	albedo.z 	= pow(albedo.z, 2.2);
	
	vec3 normal		= texture(normalMap, texcoord).xyz;
	vec3 position	= texture(positionMap, texcoord).xyz;
	
	//discard unnessisary fragmnets
	if(length(position)==0)
	{
		discard;
	}
	
	float metallic		= texture(materialMap, texcoord).g;
	float roughness		= texture(materialMap,texcoord).r;
	
	
	normal = normal * 2.0 - 1.0; //unpack normal.
	vec3 N = normal;
	vec3 V = normalize(camPos - position);
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0,albedo,metallic);
	
	//----------------------------------------------------------
	//			Calculate Lighing
	//----------------------------------------------------------
	
	//Calculate Each light (will be removed when lighting is refactored)
	vec3 Lo = vec3(0.0);
	
		//Calculate per-light directional radiance
		vec3 L 			  = normalize(lightDirection);
		vec3 H			  = normalize(V + L);
		vec3 radiance 	  = lightColor;
		
		
		//cook-torrance brdf
		float NDF	= DistributionGGX(N,H,roughness);
		float G		= GeometrySmith(N,V,L,roughness);
		vec3 F		= fresnelSchlick(max(dot(H,V),0.0), F0);
		
		vec3 nominator		= NDF * G * F;
		float denominator	= 4 * max(dot(N,V),0.0) * max(dot(N,L), 0.0) + 0.001; //add 0.001 to prevent divide by zero
		vec3 specular		= nominator / denominator;
		
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;
		

		//add to outgoing radiance Lo
		float NdotL = max(dot(N,L),0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	
	
	//-----------------------------------------------------------
	//			Add to Total Lighing
	//-----------------------------------------------------------
	
	//if the fragment is in shadow, do not add light
	float shadow = ShadowCalculation(lightSpaceMatrix * vec4(position.xyz, 1), normal);
	FragColor = vec4(Lo * (1.0 - shadow), 1.0);
}