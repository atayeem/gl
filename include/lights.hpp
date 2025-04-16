#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>

using glm::vec3;
using std::string;
using sampler2D = int;

struct DirLight {
    string name;

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    void use(const Shader& shader) {
        shader.set_vec3v(name + ".direction", direction);
        shader.set_vec3v(name + ".ambient", ambient);
        shader.set_vec3v(name + ".diffuse", diffuse);
        shader.set_vec3v(name + ".specular", specular);
    }
};

struct PointLight {
    string name;

    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    void use(const Shader& shader) {
        shader.set_vec3v(name + ".position", position);

        shader.set_float(name + ".constant", constant);
        shader.set_float(name + ".linear", linear);
        shader.set_float(name + ".quadratic", quadratic);

        shader.set_vec3v(name + ".ambient", ambient);
        shader.set_vec3v(name + ".diffuse", diffuse);
        shader.set_vec3v(name + ".specular", specular);
    }
};

struct Material {
    string name;

    sampler2D texture;
    sampler2D diffuse;
    sampler2D specular;

    float     shininess;

    void use(const Shader& shader) {
        shader.set_sampler2D(name + ".texture", texture);
        shader.set_sampler2D(name + ".diffuse", diffuse);
        shader.set_sampler2D(name + ".specular", specular);
        shader.set_float(name + ".shininess", shininess);
    }
};