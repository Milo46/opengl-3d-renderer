#version 460

layout (location = 0) in vec3 a_Position;

out vec3 vertexPosition;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
    vertexPosition = a_Position;
}
