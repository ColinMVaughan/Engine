#version 420

in vec2 texcoord;
out vec4 FragColor;

uniform vec3 camPos;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D positionMap;

uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D 	brdfLUT;

//lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

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

void main()
{
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
	
	float metallic		= texture(metallicMap, texcoord).r;
	float roughness		= texture(roughnessMap,texcoord).r;
	float ao			= texture(aoMap, texcoord).r;
	
	
	normal = normal * 2.0 - 1.0; //unpack normal.
	vec3 N = normal;
	vec3 V = normalize(camPos - position);
	vec3 R = reflect(-V,N);
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0,albedo,metallic);
	
	//Calculate Each light (will be removed when lighting is refactored)
	vec3 Lo = vec3(0.0);
	for(int i=0; i< 1; ++i)
	{
		//Calculate per-light radiance
		vec3 L 			  = normalize(lightPositions[i] - position);
		vec3 H			  = normalize(V + L);
		float distance    = length(lightPositions[i] - position);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance 	  = lightColors[i] * attenuation;
		
		
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
	}

	//ambiant lighting
	vec3 F = fresnelSchlickRoughness(max(dot(N,V),0.0), F0, roughness);
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;
	
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;
	
	//sample both pre-filter map and BRDF lut & combine.
	const float MAX_REFLECTION_LOD = 5.0;
	vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf = texture(brdfLUT, vec2(max(dot(N,V), 0.0), roughness)).rg;
	vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
	
	vec3 ambiant = (kD * diffuse + specular) * ao;
	
	
	//Combine ambiant color with summed light influence
	vec3 color = ambiant + Lo;
	
	//Gamma correct color
	color = color / (color + vec3(1.0));
	color = pow(color,vec3(1.0/2.2));
	
	FragColor = vec4(color,1.0);
}