#version 460

out vec4 FragColor;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexcoord;

struct Light
{
    vec3 Color;
    vec3 Position;
};

uniform Light u_Light;

struct Material
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

uniform Material u_Material;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;
uniform sampler2D u_EmissionTexture;

uniform vec3 u_ViewPosition;

void main()
{
    // #1. Ambient lighting.
    vec3 ambient = u_Light.Color * u_Material.Ambient * vec3(texture(u_DiffuseTexture, vertexTexcoord));

    // #2. Diffuse lighting.
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(u_Light.Position - vertexPosition);
    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.Color * diffuseStrength * u_Material.Diffuse * vec3(texture(u_DiffuseTexture, vertexTexcoord));

    // #3. Specular lighting.
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPosition - vertexPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
    vec3 specular = u_Light.Color * spec * u_Material.Specular * vec3(texture(u_SpecularTexture, vertexTexcoord));

    // #4. Emission.
    vec3 emission = vec3(texture(u_EmissionTexture, vertexTexcoord));

    // #5. Everything combined.
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}
