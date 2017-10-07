#version 410 core

// define the number of CPs in the output patch
layout (vertices = 3) out;

uniform vec3 viewPos;

// attributes of the input CPs
in vec3 CS_WorldPos[];
in vec2 CS_TexCoords[];
in vec3 CS_Normal[];
in vec3 CS_Tangent[];

// attributes of the output CPs
out vec3 ES_WorldPos[];
out vec2 ES_TexCoords[];
out vec3 ES_Normal[];
out vec3 ES_Tangent[];

float GetTessLevel(float Distance0, float Distance1) {

    float AvgDistance = (Distance0 + Distance1) / 2.0;

    if (AvgDistance <= 2.0) {
        return 10.0;
    }
    else if (AvgDistance <= 5.0) {
        return 7.0;
    }
    else {
        return 3.0;
    }
}

void main() {

    // Set the control points of the output patch
    ES_TexCoords[gl_InvocationID] = CS_TexCoords[gl_InvocationID];
    ES_Normal[gl_InvocationID] = CS_Normal[gl_InvocationID];
    ES_WorldPos[gl_InvocationID] = CS_WorldPos[gl_InvocationID];
    ES_Tangent[gl_InvocationID] = CS_Tangent[gl_InvocationID];

    // Calculate the distance from the camera to the three control points
    float EyeToVertexDistance0 = distance(viewPos, ES_WorldPos[0]);
    float EyeToVertexDistance1 = distance(viewPos, ES_WorldPos[1]);
    float EyeToVertexDistance2 = distance(viewPos, ES_WorldPos[2]);

    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];

}
