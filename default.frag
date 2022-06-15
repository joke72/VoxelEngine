#version 460 core
out vec4 FragColor;

in vec3 normal;
flat in int blockID;

void main()
{
    vec3 result = (normal + 1) * 0.5;
    
    vec3 color = vec3(0.0);
    if(blockID == 0)
        color = vec3(1.0, 0.5, 0.2);
    if(blockID == 1)
        color = vec3(0.2, 1.0, 0.5);

    FragColor = vec4(result, 1.0);//vec4(0.2, 0.5, 1.0, 1.0);
}