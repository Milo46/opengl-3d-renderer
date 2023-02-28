#version 460

out vec4 FragColor;

in vec2 vertexTexcoord;

uniform sampler2D u_ScreenTexture;

void main()
{
    FragColor = texture(u_ScreenTexture, vertexTexcoord);
}
