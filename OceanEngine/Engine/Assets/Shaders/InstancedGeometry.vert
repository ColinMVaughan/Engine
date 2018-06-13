#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

layout(location = 3) in mat4 in_offset;
layout(location = 7) in uint in_index;

out vec2 texcoord;
out vec3 norm;
out vec3 pos;
out flat uint materialIndex;

void main()
{
	texcoord = in_uv;
	norm =  mat3(uModel) * in_normal;
	
	vec4 viewSpace = uView * uModel * in_offset * vec4(in_vert,1.0f);
	vec4 modelSpace = uModel * in_offset * vec4(in_vert,1.0f);

	gl_Position = uProj * viewSpace;
	
	pos = modelSpace.xyz;
	
	materialIndex = in_index;
}