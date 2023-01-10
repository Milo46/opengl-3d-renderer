#version 460

out vec4 FragColor;

in vec2 vertexTexcoord;

uniform vec3 u_Color;

void main()
{
    FragColor = vec4(u_Color, 1.0);
}
