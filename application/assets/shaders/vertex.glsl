#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexNormal;
out vec2 vertexTexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexNormal = aNormal;
    vertexTexCoord = aTexCoord;
}
