#version 460

out vec4 FragColor;

uniform float u_Time;

void main()
{
    FragColor = vec4(vec3(abs(sin(u_Time))), 1.0);
}
