#version 430 core
out vec4 FragColor;
	
in vec2 TexCoords;
	
uniform sampler2D tex;
	
void main()
{             
    vec3 texCol = texture(tex, TexCoords).rgb;  
	
	float exposure = 1.0;
	float gamma = 1.2;
	
    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-texCol * exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
	
    FragColor = vec4(mapped, 1.0);
}