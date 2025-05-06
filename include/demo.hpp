#ifndef _DEMO_HPP
#define _DEMO_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

#include "camera.hpp"

class Window {
public: 
    // camera
    Camera camera;
    GLFWwindow *window;

    int screen_width;
    int screen_height;

    float lastX;
    float lastY;
    bool firstMouse = true;
    
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Window () :
        camera(glm::vec3(0.0f, 0.0f, 3.0f)) {

            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(500, 500, "13_diffuse_maps", NULL, NULL);

            if (window == NULL)
            {
                std::cerr << "Failed to create GLFW window" << std::endl;
                std::exit(-1);
            }

            glfwGetWindowSize(window, &screen_width, &screen_height);
        
            lastX = static_cast<float>(screen_width) / 2;
            lastY = static_cast<float>(screen_height) / 2;

            glfwMakeContextCurrent(window);

            glfwSetFramebufferSizeCallback(window, cb_window_size);
            glfwSetCursorPosCallback(window, cb_mouse);
            glfwSetScrollCallback(window, cb_scroll);

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            glfwSetWindowUserPointer(window, this);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cerr << "Failed to initialize GLAD" << std::endl;
                std::exit(-1);
            }
    
            glEnable(GL_DEPTH_TEST);
        }

    ~Window () {
        glfwTerminate();
    }

    void loop(void (*render_loop)(glm::mat4& projection, glm::mat4& view))
    {
        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
    
            processInput(window);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    
            glm::mat4 projection = glm::perspective(
                glm::radians(camera.Zoom), 
                static_cast<float>(screen_width) / static_cast<float>(screen_height), 0.1f, 100.0f);
            
            glm::mat4 view = camera.GetViewMatrix();
            
            render_loop(projection, view);
    
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    
    void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
    }
    
    void update_size(int width, int height) {
        screen_width = width;
        screen_height = height;
    }

    static void cb_window_size(GLFWwindow* glfw_window, int width, int height)
    {
        Window *window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        glViewport(0, 0, width, height);
        window->update_size(width, height);
    }
    
    static void cb_mouse(GLFWwindow* glfw_window, double xposIn, double yposIn)
    {
        Window *w = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
    
        if (w->firstMouse)
        {
            w->lastX = xpos;
            w->lastY = ypos;
            w->firstMouse = false;
        }
    
        float xoffset = xpos - w->lastX;
        float yoffset = w->lastY - ypos; // reversed since y-coordinates go from bottom to top
    
        w->lastX = xpos;
        w->lastY = ypos;
    
        w->camera.ProcessMouseMovement(xoffset, yoffset);
    }
    
    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    static void cb_scroll(GLFWwindow* glfw_window, double, double yoffset)
    {
        Window *w = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

        w->camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
};


#endif