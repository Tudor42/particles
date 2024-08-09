#include "./application.h"
#include "gui/OpenGL/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "simulation/models/BoxBoundary.h"
#include "gui/Renderer2d.h"
#include "simulation/Simulation.h"

Application *Application::instance = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_move_callback(GLFWwindow* window, double xMousePos, double yMousePos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void Application::run() {
    BoxBoundary::lowerBoundary = glm::vec3(-50., -50., 0);
    BoxBoundary::upperBoundary = glm::vec3(50, 50, 0);
    Renderer2d::initShaders("resources/shaders");

    while (!glfwWindowShouldClose(m_window)) {
        double currentTime = glfwGetTime();
       
        if (currentTime - m_lastTime >= 1.0/60.0){
            Simulation::step();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Renderer2d::renderBoundingBox();
            
            glfwSwapBuffers(m_window);

            glfwPollEvents();
            m_lastTime = currentTime;
        }
    }
};

void Application::closeWindow() {
    glfwSetWindowShouldClose(m_window, 1);
    delete Application::instance;
}

Application::Application(int width, int height) {

    if (!glfwInit())
    {
        std::exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, "Particle-based simulation", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(m_window);
    glewInit();
    
    // Setting callbacks
	glfwGetFramebufferSize(m_window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetCursorPosCallback(m_window, mouse_move_callback);


    Camera2D::setHeight(m_height);
    Camera2D::setWidth(m_width);
    // glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Application::getInstance()->onFramebufferSizeChanged(width, height);
    Camera2D::setHeight(height);
    Camera2D::setWidth(width);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera2D::zoom(yoffset * 0.1);
}

void mouse_move_callback(GLFWwindow* window, double xMousePos, double yMousePos) {

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}