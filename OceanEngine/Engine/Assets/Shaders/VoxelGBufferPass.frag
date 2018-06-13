#version 420

uniform sampler2D materialAtlas;

in flat uint materialIndex;
in vec2 texcoord;
in vec3 norm;
in vec3 pos;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormals;
layout (location = 2) out vec3 outPosition;

layout (location = 3) out vec4 outRoughness;
layout (location = 4) out vec4 outMetallic;
layout (location = 5) out vec4 outAo;
layout (location = 6) out vec4 outEmissive;



void main()
{
	float temp = uintBitsToFloat(materialIndex);
	uint newMaterialIndex = uint(temp);
	
	outColor.rgb = texelFetch(materialAtlas, ivec2(0,newMaterialIndex), 0).rgb;
	outColor.a = 1.0;
	
	vec3 material = texelFetch(materialAtlas, ivec2(1,newMaterialIndex), 0).rgb;
	
	
	outAo.r = 1.0;
	outMetallic.r = material.r;
	outRoughness.r = material.g;
	outEmissive.r = material.b;
	

	//Pack Normals
		//in-> [-1,1]
		//out->[0,1]
	outNormals = norm * 0.5 + 0.5;

	//viewSpace Positions
	outPosition = pos;

}