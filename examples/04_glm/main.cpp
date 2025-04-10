/* This program prints 210 to demonstrate vector transformations. */

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);

    // Identity matrix 4x4
    glm::mat4 trans = glm::mat4(1.0f);

    // Transform using homogenous matrix
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;

    return 0;
}