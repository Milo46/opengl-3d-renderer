#version 460

out vec4 FragColor;

uniform float u_Time;

in vec3 vertexPosition;

void main()
{
    // FragColor = vec4(vec3(abs(sin(u_Time))), 1.0);
    FragColor = vec4(abs(vertexPosition), 1.0);
}
