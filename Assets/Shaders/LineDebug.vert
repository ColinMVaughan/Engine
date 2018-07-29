#version 420 

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform vec3 inColour;

layout(location = 0) in vec3 in_vert;

out vec4 colour;

void main()
{
	//colour = vec4((in_colour & 0x000000FF)>>0,
	//(in_colour & 0x0000FF00)>>8,
	//(in_colour & 0x00FF0000)>>16,
	//(in_colour & 0xFF000000)>>24);

	colour.rgb = inColour.rgb;
	colour.a = 1.0;
	
	vec4 viewSpace = uView * uModel *vec4(in_vert,1.0f);
	gl_Position = uProj * viewSpace;
}