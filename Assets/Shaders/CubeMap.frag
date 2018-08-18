#version 420
out vec4 FragColor;

in vec3 localPos;
  
uniform samplerCube environmentMap;
uniform float Exposure;
  
void main()
{
    vec3 envColor = texture(environmentMap, localPos).rgb;
    
    //gamma correct & tonemap w/ exposure
	vec3 mapped = vec3(1.0) - exp(-envColor * Exposure);
    
    mapped.x = max(0, mapped.x - 0.004);
	mapped.y = max(0, mapped.y - 0.004);
	mapped.z = max(0, mapped.z - 0.004);
    mapped = (mapped * (6.2*mapped + 0.5))/(mapped*(6.2*mapped + 1.7) + 0.06);
    //mapped = pow(mapped, vec3(1.0/2.2)); 

    FragColor = vec4(mapped, 1.0);
}