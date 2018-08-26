#version 420

in vec2 texcoord;
out vec4 FragColor;

uniform vec3 camPos;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D positionMap;

uniform sampler2D materialMap;
uniform sampler2D aoMap;


uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D 	brdfLUT;

uniform sampler2D combinedLights;



const float PI = 3.14159265359;

uniform float Exposure;
const float A = 0.22; //Shoulder Strength
const float B = 0.30; //Linear Strength
const float C = 0.10; //Linear Angle
const float D = 0.20; //Toe Strength
const float E = 0.01; //Toe Numerator
const float F = 0.30; //Toe Denominator
const float LinearWhite = 11.2; //Linear White point value 

vec3 FilmicTonemapUncharted(vec3 color)
{
    color = ((color*(A*color+C*B)+D*E)/(color*(A*color+B)+D*F)) - E/F;
    color = F*color / F*LinearWhite;

    // Gamma Correction
   color = pow(color, vec3(1.0/2.2));
   return color;
}

vec3 FilmicTonemapDefault(vec3 color)
{
    color.x = max(0, color.x - 0.004);
	color.y = max(0, color.y - 0.004);
	color.z = max(0, color.z - 0.004);
    color = (color * (6.2*color + 0.5))/(color*(6.2*color + 1.7) + 0.06);

	return color;
}

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

	//----------------------------------------------
	// Collect Data from Textures
	//----------------------------------------------
	//convert sRGB to linear color-space
	vec3 albedo	= texture(albedoMap, texcoord).rgb;
	albedo.x 	= pow(albedo.x, 2.2);
	albedo.y	= pow(albedo.y, 2.2);
	albedo.z 	= pow(albedo.z, 2.2);
	
	vec3 normal		   = texture(normalMap, texcoord).xyz;
	vec3 position	   = texture(positionMap, texcoord).xyz;
	vec3 CombinedLight = texture(combinedLights, texcoord).rgb;
	
	//discard unnessisary fragmnets
	if(length(position)==0)
	{
		discard;
	}
	
	float roughness		= texture(materialMap,texcoord).r;
	float metallic		= texture(materialMap, texcoord).g;
	float emissive		= texture(materialMap, texcoord).b;
		
	float ao			= texture(aoMap, texcoord).r;

	
	normal = normal * 2.0 - 1.0; //unpack normal.
	vec3 N = normal;
	vec3 V = normalize(camPos - position);
	vec3 R = reflect(-V,N);
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0,albedo,metallic);
	
	//------------------------------------------------------
	// 
	//------------------------------------------------------

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
	
	
	//Combine ambiant color with summed light influence & multiply against emissive influence
	vec3 color = (ambiant + CombinedLight) * float(1.0f - emissive) + (albedo)*emissive;
	
	//Gamma correct & tonemap color /w exposure
	//color = color / (color + vec3(1.0));
	vec3 mapped = vec3(1.0) - exp(-color * Exposure);
	mapped = FilmicTonemapUncharted(mapped);
	
	FragColor = vec4(mapped,1.0);
}