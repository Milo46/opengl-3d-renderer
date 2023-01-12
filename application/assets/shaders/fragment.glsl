#version 460

out vec4 FragColor;

in vec2 vertexTexcoord;

uniform vec3 u_Color;
uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, vertexTexcoord) * vec4(u_Color, 1.0);
}
