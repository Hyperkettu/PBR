#version 410 core

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct Material {
    sampler2D albedo;
    sampler2D normalMap;
    sampler2D ao;
    sampler2D roughness;
    sampler2D metallic;
};

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in mat3 TBN;

out vec4 FragColor;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

uniform vec3 viewPos;
uniform DirLight sun;
uniform Material material;

const float PI = 3.14159265359f;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH*NdotH;
    
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
    
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r*r) / 8.0f;
    
    float nom   = NdotV;
    float denom = NdotV * (1.0f - k) + k;
    
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 N, vec3 V, vec3 F0, vec3 albedo, float roughness, float metallic)
{
    // calculate per-light radiance
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = light.color;
    
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);
    
    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001f; // 0.001 to prevent divide by zero.
    vec3 specular = nominator / denominator;
    
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0f) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0f - metallic;
    
    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0f);
    
    // add to outgoing radiance Lo
    return (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

void main()
{
    
    
    
    // sample normal map
    vec3 N = texture(material.normalMap, TexCoords).rgb;
    vec3 albedo = texture(material.albedo, TexCoords).rbg;
    float ao = texture(material.ao, TexCoords).r;
    float roughness = texture(material.roughness, TexCoords).r;
    float metallic = texture(material.metallic, TexCoords).r;
  
    // transform sampled normal to range [-1, 1]
    N = normalize(N * 2.0 - vec3(1.0f, 1.0f, 1.0f));//1.0);
    
    // transform normals from tangent space to world space
    N = normalize(TBN * N);
   
    
 //   vec3 N = Normal;
    vec3 V = normalize(viewPos - WorldPos);
    vec3 R = reflect(-V, N);
    
    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, albedo, metallic);
    
    // calculate sun
    vec3 Lo = CalcDirLight(sun, N, V, F0, albedo, roughness, metallic);
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic;
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 5.0f;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0f), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
    
    vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 color = ambient + Lo;
    
    // HDR tonemapping
    color = color / (color + vec3(1.0f));
    // gamma correct
    color = pow(color, vec3(1.0f/2.2f));
    
    FragColor = vec4(color , 1.0f);
}
