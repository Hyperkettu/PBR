#version 330 core

in vec3 Color;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D stexture;

void main()
{
    color = texture(stexture, TexCoord) * vec4(Color, 1.0f);
}
