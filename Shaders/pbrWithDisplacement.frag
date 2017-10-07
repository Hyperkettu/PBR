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
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

uniform vec3 viewPos;
uniform DirLight sun;
uniform Material material;
uniform mat4 projection;

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

float sqr(float x) { return x*x; }

float schlick_fresnel(float u)
{
    float m = clamp(1-u, 0, 1);
    float m2 = m*m;
    return m2*m2*m; // pow(m,5)
}

float GTR1(float NdotH, float a)
{
    if (a >= 1) return 1/PI;
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return (a2-1) / (PI*log(a2)*t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return a2 / (PI * t*t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
    return 1 / (PI * ax*ay * sqr( sqr(HdotX/ax) + sqr(HdotY/ay) + NdotH*NdotH ));
}

float smithG_GGX(float NdotV, float alphaG)
{
    float a = alphaG*alphaG;
    float b = NdotV*NdotV;
    return 1 / (NdotV + sqrt(a + b - a*b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
    return 1 / (NdotV + sqrt( sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV) ));
}

vec3 rotate_vector(vec3 p, vec3 n, float theta) {
    return (
	           p * cos(theta) + cross(n, p) *
	           sin(theta) + n * dot(p, n) *
	           (1.0 - cos(theta))
            );
}

vec3 mon2lin(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

float fresnel_dielectric_0(float eta)
{
    /* compute fresnel reflactance at normal incidence => cosi = 1.0 */
    float A = (eta - 1.0) / (eta + 1.0);
    
    return A * A;
}

float fresnel_dielectric_cos(float cosi, float eta)
{
    /* compute fresnel reflectance without explicitly computing
     * the refracted direction */
    float c = abs(cosi);
    float g = eta * eta - 1.0 + c * c;
    float result;
    
    if (g > 0.0) {
        g = sqrt(g);
        float A = (g - c) / (g + c);
        float B = (c * (g + c) - 1.0) / (c * (g - c) + 1.0);
        result = 0.5 * A * A * (1.0 + B * B);
    }
    else {
        result = 1.0;  /* TIR (no refracted component) */
    }
    
    return result;
}

float fresnel_dielectric(vec3 Incoming, vec3 Normal, float eta)
{
    /* compute fresnel reflectance without explicitly computing
     * the refracted direction */
    return fresnel_dielectric_cos(dot(Incoming, Normal), eta);
}

void node_bsdf_principled(vec4 base_color, float subsurface, vec3 subsurface_radius, vec4 subsurface_color, float metallic, float specular,
                          float specular_tint, float roughness, float anisotropic, float anisotropic_rotation, float sheen, float sheen_tint, float clearcoat,
                          float clearcoat_roughness, float ior, float transmission, float transmission_roughness, vec3 N, vec3 CN, vec3 T, vec3 I, out vec3 result)
{
    /* ambient light */
    // TODO: set ambient light to an appropriate value
    vec3 L = mix(0.1, 0.03, metallic) * mix(base_color.rgb, subsurface_color.rgb, subsurface * (1.0 - metallic));
    
    float eta = (2.0 / (1.0 - sqrt(0.08 * specular))) - 1.0;
    
    /* set the viewing vector */
    vec3 V = (projection[3][3] == 0.0) ? -normalize(I) : vec3(0.0, 0.0, 1.0);
    
    /* get the tangent */
    vec3 Tangent = T;
    if (T == vec3(0.0)) {
        // if no tangent is set, use a default tangent
        if(N.x != N.y || N.x != N.z) {
            Tangent = vec3(N.z-N.y, N.x-N.z, N.y-N.x);  // (1,1,1) x N
        }
        else {
            Tangent = vec3(N.z-N.y, N.x+N.z, -N.y-N.x);  // (-1,1,1) x N
        }
    }
    
    /* rotate tangent */
    if (anisotropic_rotation != 0.0) {
        Tangent = rotate_vector(Tangent, N, anisotropic_rotation * 2.0 * PI);
    }
    
    /* calculate the tangent and bitangent */
    vec3 Y = normalize(cross(N, Tangent));
    vec3 X = cross(Y, N);
    
    /* fresnel normalization parameters */
    float F0 = fresnel_dielectric_0(eta);
    float F0_norm = 1.0 / (1.0 - F0);
    
    /* directional lights */
    for (int i = 0; i < 1; i++) {
        vec3 light_position_world = -sun.direction;
        vec3 light_position = normalize(light_position_world);
        
        vec3 H = normalize(light_position + V);
        
        vec3 light_diffuse = sun.color;
        vec3 light_specular = sun.color;
        
        float NdotL = dot(N, light_position);
        float NdotV = dot(N, V);
        float LdotH = dot(light_position, H);
        
        vec3 diffuse_and_specular_bsdf = vec3(0.0);
        if (NdotL >= 0.0 && NdotV >= 0.0) {
            float NdotH = dot(N, H);
            
            float Cdlum = 0.3 * base_color.r + 0.6 * base_color.g + 0.1 * base_color.b; // luminance approx.
            
            vec3 Ctint = Cdlum > 0 ? base_color.rgb / Cdlum : vec3(1.0); // normalize lum. to isolate hue+sat
            vec3 Cspec0 = mix(specular * 0.08 * mix(vec3(1.0), Ctint, specular_tint), base_color.rgb, metallic);
            vec3 Csheen = mix(vec3(1.0), Ctint, sheen_tint);
            
            // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
            // and mix in diffuse retro-reflection based on roughness
            
            float FL = schlick_fresnel(NdotL), FV = schlick_fresnel(NdotV);
            float Fd90 = 0.5 + 2.0 * LdotH*LdotH * roughness;
            float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);
            
            // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
            // 1.25 scale is used to (roughly) preserve albedo
            // Fss90 used to "flatten" retroreflection based on roughness
            float Fss90 = LdotH*LdotH * roughness;
            float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
            float ss = 1.25 * (Fss * (1.0 / (NdotL + NdotV) - 0.5) + 0.5);
            
            // specular
            float aspect = sqrt(1.0 - anisotropic * 0.9);
            float a = sqr(roughness);
            float ax = max(0.001, a / aspect);
            float ay = max(0.001, a * aspect);
            float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay); //GTR2(NdotH, a);
            float FH = (fresnel_dielectric_cos(LdotH, eta) - F0) * F0_norm;
            vec3 Fs = mix(Cspec0, vec3(1.0), FH);
            float roughg = sqr(roughness * 0.5 + 0.5);
            float Gs = smithG_GGX(NdotL, roughg) * smithG_GGX(NdotV, roughg);
            
            // sheen
            vec3 Fsheen = schlick_fresnel(LdotH) * sheen * Csheen;
            
            vec3 diffuse_bsdf = (mix(Fd * base_color.rgb, ss * subsurface_color.rgb, subsurface) + Fsheen) * light_diffuse;
            vec3 specular_bsdf = Gs * Fs * Ds * light_specular;
            diffuse_and_specular_bsdf = diffuse_bsdf * (1.0 - metallic) + specular_bsdf;
        }
        diffuse_and_specular_bsdf *= max(NdotL, 0.0);
        
        float CNdotL = dot(CN, light_position);
        float CNdotV = dot(CN, V);
        
        vec3 clearcoat_bsdf = vec3(0.0);
        if (CNdotL >= 0.0 && CNdotV >= 0.0 && clearcoat > 0.0) {
            float CNdotH = dot(CN, H);
            //float FH = schlick_fresnel(LdotH);
            
            // clearcoat (ior = 1.5 -> F0 = 0.04)
            float Dr = GTR1(CNdotH, sqr(clearcoat_roughness));
            float Fr = fresnel_dielectric_cos(LdotH, 1.5); //mix(0.04, 1.0, FH);
            float Gr = smithG_GGX(CNdotL, 0.25) * smithG_GGX(CNdotV, 0.25);
            
            clearcoat_bsdf = clearcoat * Gr * Fr * Dr * vec3(0.25) * light_specular;
        }
        clearcoat_bsdf *= max(CNdotL, 0.0);
        
        L += diffuse_and_specular_bsdf + clearcoat_bsdf;
    }
    
    result = L;
}




vec4 ogl_pbr(vec3 N, vec3 albedo, float roughness, float metallic, float ao);
vec4 disney_pbr(vec3 N, vec3 albedo, float roughness, float metallic, float ao);

void main()
{
    
    
    
    // sample normal map
    vec3 N = texture(material.normalMap, TexCoords).rgb;
    vec3 albedo = texture(material.albedo, TexCoords).rgb;
    float ao = texture(material.ao, TexCoords).r;
    float roughness = texture(material.roughness, TexCoords).r;
    float metallic = texture(material.metallic, TexCoords).r;
    
    albedo = pow(albedo, vec3(2.2f));
    
    // transform sampled normal to range [-1, 1]
    N = normalize(N * 2.0 - vec3(1.0f, 1.0f, 1.0f));//1.0);
    
    // transform normals from tangent space to world space
    N = normalize(TBN * N);
    
    FragColor = ogl_pbr(N, albedo, roughness, metallic, ao);
    //FragColor = disney_pbr(N, mon2lin(albedo), roughness, metallic, ao);
}

vec4 disney_pbr(vec3 N, vec3 albedo, float roughness, float metallic, float ao) {

    vec3 V = normalize(viewPos - WorldPos);
    vec3 R = reflect(-V, N);

    
    vec3 color;
    
    vec3 F0 = mix(vec3(0.08 * 0.5), albedo, metallic);
    
    node_bsdf_principled(vec4(albedo, 1.0), 0.0, vec3(0.1, 0.1, 0.1), vec4(albedo, 1.0), metallic, 0.5, 0.0, roughness, 0.0, 0.0, 0.0, 0.5, 0.0, 0.3, 1.45, 0.0, 0.0, N, N, Tangent, -V, color);
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic;
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = albedo * irradiance;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 5.0f;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0f), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
    
    vec3 ambient = (kD * diffuse + specular) * ao;
    
    color = ambient + color;
    
    // HDR tonemapping
    color = color / (color + vec3(1.0f));
 
    // gamma correct
    color = pow(color, vec3(1.0f/2.2f));
    
    return vec4(color, 1.0f);

}


vec4 ogl_pbr(vec3 N, vec3 albedo, float roughness, float metallic, float ao) {

    albedo = mon2lin(albedo);
    
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
    vec3 diffuse = albedo * irradiance;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 5.0f;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0f), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
    
    vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 color = ambient + Lo;
    
  //  color = albedo;
    
    // HDR tonemapping
    color = color / (color + vec3(1.0f));
    
    
    // gamma correct
    color = pow(color, vec3(1.0f/2.2f));
    
    return vec4(color , 1.0f);

}





