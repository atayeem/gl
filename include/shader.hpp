#include "glm/detail/type_mat.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertex_path, const char* fragment_path)
    {
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream v_shader_file;
        std::ifstream f_shader_file;

        v_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            v_shader_file.open(vertex_path);
            f_shader_file.open(fragment_path);

            std::stringstream v_shader_stream, f_shader_stream;

            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();

            v_shader_file.close();
            f_shader_file.close();

            vertex_code = v_shader_stream.str();
            fragment_code = f_shader_stream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::exit(-1);
        }

        const char* v_shader_code = vertex_code.c_str();
        const char* f_shader_code = fragment_code.c_str();


        GLuint vertex, fragment;
        int success;
        char info_log[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, info_log);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                      info_log << std::endl;
            std::exit(-1);
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, info_log);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
                      info_log << std::endl;
            std::exit(-1);
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, info_log);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
                      info_log << std::endl;
            std::exit(-1);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() const
    {
        glUseProgram(ID);
    }

    void set_bool(const std::string &name, bool value) const
    {
        glUniform1i(loc(name), (int) value);
    }
    void set_int(const std::string &name, int value) const
    {
        glUniform1i(loc(name), value);
    }

    void set_sampler2D(const std::string &name, int value) const
    {
        set_int(name, value);
    }

    void set_float(const std::string &name, float value) const
    {
        glUniform1f(loc(name), value);
    }

    void set_mat4(const std::string &name, glm::mat4& value) const
    {
        glUniformMatrix4fv(loc(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void set_vec3v(const std::string &name, glm::vec3& value) const
    {
        glUniform3fv(loc(name), 1, glm::value_ptr(value));
    }

    void set_vec3(const std::string &name, float a, float b, float c) const
    {
        glm::vec3 vec(a, b, c);
        set_vec3v(name, vec);
    }

private:
    GLint loc(const std::string &name) const {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        if (loc == -1) {
            std::cerr << "Failed to find uniform '" << name << "'";
            std::exit(-1);
        }

        return loc;
    }
};