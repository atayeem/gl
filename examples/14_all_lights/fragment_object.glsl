#version 330 core

#define NR_POINT_LIGHTS 4

struct Material {
    sampler2D texture;
    sampler2D diffuse;
    sampler2D specular;

    float     shininess;
};

// Forwarded from the vertex shader.
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
vec3 CalcDirLight(DirLight light, Material mat, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);

    vec3 ambient = light.ambient * vec3(texture(mat.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * vec3(texture(mat.diffuse, TexCoords));
    vec3 specular = light.specular * vec3(texture(mat.specular, TexCoords));

    return (ambient + diffuse + specular);
}

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);

    // attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient = light.ambient * vec3(texture(mat.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(mat.specular, TexCoords));

    return (ambient + diffuse + specular) * attenuation;
}

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform Material material;

uniform vec3 viewPos;

// Given to OpenGL.
out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, material, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], material, norm, FragPos, viewDir);
    
    // result += CalcSpotLight(spotLight, material, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}