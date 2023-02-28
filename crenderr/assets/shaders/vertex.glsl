#version 460

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_Texcoord;

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexcoord;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0);

    vertexPosition = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
    vertexNormal   = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;
    vertexTexcoord = a_Texcoord;
}
