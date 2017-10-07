#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D normalMap;
    float shininess;
};

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 0

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
//uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPosition);
    
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    //  for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //      result += CalcPointLight(pointLights[i], norm, FragPosition, viewDir);
    
    result += CalcSpotLight(spotLight, norm, FragPosition, viewDir);
    
    // gamma correction
    result = pow(result, vec3(1.0/2.2));
    
    color = vec4(result, 1.0);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    const float Pi = 3.14159f;
    
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = 1/Pi * max(dot(normal, lightDir), 0.0);
    // Specular shading (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = ( 8.0 + material.shininess ) / ( 8.0 * Pi ) * pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
    
    //  return ambient;
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    const float Pi = 3.14159f;
    
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = 1/Pi * max(dot(normal, lightDir), 0.0);
    // Specular shading (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = ( 8.0 + material.shininess ) / ( 8.0 * Pi ) * pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    const float Pi = 3.14159f;
    
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = 1/Pi * max(dot(normal, lightDir), 0.0);
    // Specular shading (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = ( 8.0 + material.shininess ) / ( 8.0 * Pi ) * pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
