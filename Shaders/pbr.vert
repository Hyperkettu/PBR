#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = texCoords;
    WorldPos = vec3(model * vec4(position, 1.0f));
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalize(normalMatrix * normal);
    
    vec3 T = normalize((model * vec4(tangent, 0.0f)).xyz);
    // Gram-Schmidt
    T = normalize(T - dot(T, Normal) * Normal);
    vec3 B = cross(Normal, T);
    TBN = mat3(T, B, Normal);
    
    gl_Position =  projection * view * vec4(WorldPos, 1.0f);
}

