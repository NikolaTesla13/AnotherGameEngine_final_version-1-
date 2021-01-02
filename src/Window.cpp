#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Window.h>
#include <Debugger.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glCall(glViewport(0, 0, width, height));
}

Window::Window(const std::string &title, unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = width;

    glfwInit();

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

bool Window::isOpen() const
{
    return !glfwWindowShouldClose(window);
}

void Window::startFrame() const
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::endFrame() const
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

Window::~Window()
{
    glfwTerminate();
}