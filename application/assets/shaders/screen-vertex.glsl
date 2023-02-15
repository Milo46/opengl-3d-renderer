#version 460

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_Texcoord;

out vec2 vertexTexcoord;

void main()
{
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    vertexTexcoord = a_Texcoord;
}
