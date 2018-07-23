#version 420

uniform sampler2D Albedo;
uniform sampler2D Roughness;
uniform sampler2D Metallic;
uniform sampler2D Normal;
uniform sampler2D AO;
uniform sampler2D Emissive;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormals;
layout (location = 2) out vec3 outPosition;

layout (location = 3) out vec4 outMaterial; //r=roughness, g=metallic, b=emissive


vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(Normal, texcoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(pos);
    vec3 Q2  = dFdy(pos);
    vec2 st1 = dFdx(texcoord);
    vec2 st2 = dFdy(texcoord);

    vec3 N   = normalize(norm);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{

	//albedo color output
	outColor.rgb = texture(Albedo, texcoord).rgb;
	outColor.a = 1.0;

	//roughness output
	outMaterial.r = texture(Roughness, texcoord).r;

	//Metallic output
	outMaterial.g = texture(Metallic, texcoord).r;
	
	//Emissive output
	outMaterial.b = texture(Emissive, texcoord).r;

	//Pack Normals
		//in-> [-1,1]
		//out->[0,1]
	outNormals = normalize(getNormalFromMap()) * 0.5 + 0.5;

	//viewSpace Positions
	outPosition = pos;
	

}