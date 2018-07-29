#version 420

uniform sampler2D uTex;
uniform float uThreshold;

in vec2 texcoord;
out vec3 outColor;

void main()
{
	vec3 color = texture(uTex,texcoord).rgb;

	float luminence = (color.r + color.b + color.g)/3;

	if(luminence > uThreshold)
	{
		outColor = color;
	}
	else
	{
		outColor = vec3(0.0, 0.0, 0.0);
	}
}