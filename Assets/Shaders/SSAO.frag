#version 420

out float FragColour;
in vec2 texcoord;

uniform sampler2D PositionTex;
uniform sampler2D NormalTex;
uniform sampler2D NoiseTex;

uniform vec3 Samples[64];
uniform mat4 Projection;
uniform mat4 View;

const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0);
const float radius = 0.9;
const float bias = 0.025;

void main()
{
	vec3 fragPos   = texture(PositionTex, texcoord).xyz;
	vec3 normal	   = normalize(texture(NormalTex, texcoord).rgb);
	vec3 randomVec = normalize(texture(NoiseTex, texcoord * noiseScale).xyz);
	
	vec3 tangent   = normalize(randomVec - normal * dot(randomVec,normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal);
	
	//fragPos = vec3(View * vec4(fragPos, 1.0));
	
	float occlusion = 0.0;
	for(int i=0; i < 64; ++i)
	{
		vec3 mysample = TBN * Samples[i];
		mysample = fragPos + mysample * radius;
		
		vec4 offset = vec4(mysample, 1.0);
		offset = Projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;
		
		//vec3 temp = vec3(View * vec4(texture(PositionTex,offset.xy).xyz, 1.0));
		vec3 temp = texture(PositionTex,offset.xy).xyz;
		
		float sampleDepth = temp.z;
		float rangeCheck = smoothstep(0.0,1.0,radius/abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= mysample.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion/64);
	FragColour = occlusion;
}