#version 330 core
struct Material {
    sampler2D diffuse;
    vec3      specular;
    float     shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;
uniform sampler2D tex;

// Forwarded from the vertex shader.
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Given to OpenGL.
out vec4 FragColor;

void main() {

    // Ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
        
    vec3 result = (ambient + diffuse + specular) * vec3(texture(tex, TexCoord));

    FragColor = vec4(result, 1.0);
}