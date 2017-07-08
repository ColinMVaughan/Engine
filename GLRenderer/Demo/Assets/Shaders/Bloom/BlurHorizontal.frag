#version 420 

uniform sampler2D uTex;
uniform float uPixelSize;

in vec2 texcoord;
out vec3 outColor;

void main()
{
	outColor = vec3(0, 0, 0);

	outColor += texture(uTex,vec2(texcoord.x - 4.0 * uPixelSize, texcoord.y)).rgb * 0.06;
	outColor += texture(uTex,vec2(texcoord.x - 3.0 * uPixelSize, texcoord.y)).rgb * 0.09;
	outColor += texture(uTex,vec2(texcoord.x - 2.0 * uPixelSize, texcoord.y)).rgb * 0.12;
	outColor += texture(uTex,vec2(texcoord.x -		 uPixelSize, texcoord.y)).rgb * 0.15;
	outColor += texture(uTex,vec2(texcoord.x,					 texcoord.y)).rgb * 0.16;
	outColor += texture(uTex,vec2(texcoord.x +		 uPixelSize, texcoord.y)).rgb * 0.15;
	outColor += texture(uTex,vec2(texcoord.x + 2.0 * uPixelSize, texcoord.y)).rgb * 0.12;
	outColor += texture(uTex,vec2(texcoord.x + 3.0 * uPixelSize, texcoord.y)).rgb * 0.09;
	outColor += texture(uTex,vec2(texcoord.x + 4.0 * uPixelSize, texcoord.y)).rgb * 0.06;
}