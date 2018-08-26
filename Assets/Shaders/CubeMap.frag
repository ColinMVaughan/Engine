#version 420
out vec4 FragColor;

in vec3 localPos;
  
uniform samplerCube environmentMap;
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

void main()
{
    vec3 envColor = texture(environmentMap, localPos).rgb;
    
    //gamma correct & tonemap w/ exposure
	vec3 mapped = vec3(1.0) - exp(-envColor * Exposure);    
    mapped = FilmicTonemapUncharted(mapped);

    FragColor = vec4(mapped, 1.0);
}