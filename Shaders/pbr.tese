#version 410 core

layout(triangles, equal_spacing, cw) in;

struct Material {
    sampler2D displacement;
};

uniform mat4 view;
uniform mat4 projection;
uniform Material material;
uniform sampler2D displacementMap;
uniform float dispFactor;
uniform vec3 viewPos;

in vec3 ES_WorldPos[];
in vec2 ES_TexCoords[];
in vec3 ES_Normal[];
in vec3 ES_Tangent[];

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;
out vec3 Bitangent;
out vec3 Tangent;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) {
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main() {

    // Interpolate the attributes of the output vertex using the barycentric coordinates
    TexCoords = interpolate2D(ES_TexCoords[0], ES_TexCoords[1], ES_TexCoords[2]);
    Normal = interpolate3D(ES_Normal[0], ES_Normal[1], ES_Normal[2]);
    Normal = normalize(Normal);
    WorldPos = interpolate3D(ES_WorldPos[0], ES_WorldPos[1], ES_WorldPos[2]);
    vec3 T = interpolate3D(ES_Tangent[0], ES_Tangent[1], ES_Tangent[2]);
    T = normalize(T - dot(T, Normal) * Normal);
    vec3 B = cross(Normal, T);
    TBN = mat3(T, B, Normal);
    Tangent = T;
    Bitangent = B;

    if(distance(viewPos, WorldPos) < 200.0f) {
        // displacement mapping
        float disp = texture(displacementMap, TexCoords.xy).x;
        WorldPos += Normal * disp * dispFactor;
    }

    gl_Position = projection * view * vec4(WorldPos, 1.0f);
}
