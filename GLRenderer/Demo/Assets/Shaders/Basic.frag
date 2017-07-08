#version 420

uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

void main()
{
	outColor = texture(uTex, texcoord);
}