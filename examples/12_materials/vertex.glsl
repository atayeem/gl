#version 330 core

out vec3 Normal;
out vec3 FragPos;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    FragPos = vec3(model * vec4(aPos, 1.0));
}