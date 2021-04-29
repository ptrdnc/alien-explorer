#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec3 FragPos;
in vec2 texCoord;

uniform vec3 viewPos;
uniform Material material;

void main()
{
    vec3 ambient = texture(material.diffuse, texCoord).rgb;
    FragColor = vec4(ambient, 1.0);
}