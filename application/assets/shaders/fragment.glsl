#version 460

out vec4 FragColor;

uniform vec3 u_Color;

void main()
{
    FragColor = vec4(u_Color, 1.0);
}
