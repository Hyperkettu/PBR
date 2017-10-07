#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 CS_TexCoords;
out vec3 CS_WorldPos;
out vec3 CS_Normal;
out vec3 CS_Tangent;

uniform mat4 model;

void main()
{
    CS_TexCoords = texCoords;
    CS_WorldPos = vec3(model * vec4(position, 1.0f));
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    CS_Normal = normalize(normalMatrix * normal);
    
    CS_Tangent = normalize((model * vec4(tangent, 0.0f)).xyz);
    
    // Gram-Schmidt
    //T = normalize(T - dot(T, Normal) * Normal);
    // vec3 B = cross(Normal, T);
    //TBN = mat3(T, B, Normal);
}

