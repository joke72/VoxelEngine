#version 460 core
out vec4 FragColor;

in vec3 normal;
flat in int blockID;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 128
uniform PointLight pointLights[NR_POINT_LIGHTS];


uniform int activePointLights;
uniform vec3 uViewPos;

uniform sampler2D shadowMap;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color);
float ShadowCalculation(vec4 fragPosLightSpace);


in vec3 FragPos;
in vec4 FragPosLightSpace;


void main()
{
    vec3 norm = (normal + 1) * 0.5;
    
    vec3 color = vec3(0.0);
    if(blockID == 1)
        color = vec3(0.0705882, 0.360784, 0.0745098);
    if(blockID == 2)
        color = vec3(0.243137, 0.486275, 0.0901961);
        
    vec3 viewDir = normalize(uViewPos-FragPos);

    vec3 result = CalcDirLight(dirLight, normal, viewDir, color);


    

    for(int i = 0; i < activePointLights; i++)
    {
        //result += CalcPointLight(pointLights[i], normal, FragPos, viewDir, color);
    }


    FragColor = vec4(result, 1.0);//vec4(0.2, 0.5, 1.0, 1.0);
}


float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    //float bias = 0.005;//max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    // check whether current frag pos is in shadow
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - 0.00 > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}  

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color)
{
    //vec3 viewDir = normalize(-FragPos);

    vec3 lightDir = normalize(light.direction);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float spec;
    float shininess = 32.0;

    spec = pow(max(dot(normal, halfwayDir), 0.0), shininess*2);
    
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse   * diff;
    vec3 specular = light.specular * spec * (blockID == 1 ? 1.0 : 0);

    float shadow = ShadowCalculation(FragPosLightSpace, lightDir);

    return (ambient + (1.0 - shadow) * (diffuse + specular)) * color;


}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    //vec3 viewDir = normalize(-FragPos);

    vec3 lightDir = normalize(light.position - FragPos);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float spec;
    float shininess = 32.0;

    spec = pow(max(dot(normal, halfwayDir), 0.0), shininess*2);
    
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 ambient = light.ambient   *        color*0.5;
    vec3 diffuse = light.diffuse   * diff * color;
    vec3 specular = light.specular * spec * (blockID == 1 ? 1.0 : 0);

    return (ambient + diffuse + specular) * attenuation;


}