#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 Normal; // normal in world position
out vec3 FragPosition; // fragment position in world coordinates
out vec2 TexCoords;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPosition = vec3(model * vec4(position, 1.0f));
    //Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
  //  vec3 T = normalize(normalMatrix * tangent);
    vec3 T = normalize((model * vec4(tangent, 0.0f)).xyz);
  //  vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);
    
    // Gram-Schmidt
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);
    
    TBN = mat3(T, B, N);
    Normal = N;
    
}
