#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Texture {
    private:
    GLuint ID;

    public:
    Texture(const char *path) {
        stbi_set_flip_vertically_on_load(true);
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        
        GLint format = GL_RGBA;
        
        if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
            std::cerr << path << ": " << nrChannels << " channels\nWhat do I do with this?" << std::endl;

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
                format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        } else {
            std::cerr << "Failed to load texture image" << std::endl;
            throw;
        }
    }

    void param(GLint parameter, GLint value) const {
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, parameter, value);
    }

    GLuint id() const { return ID; }

    void use_as(GLint texture_unit) const {
        glActiveTexture(texture_unit);
        glBindTexture(GL_TEXTURE_2D, ID); 
    }
};