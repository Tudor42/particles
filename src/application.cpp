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
    BoxBoundary::lowerBoundary = glm::vec3(-500, -500, 0);
    BoxBoundary::upperBoundary = glm::vec3(500, 500, 0);
    
    Renderer2d::init("resources/shaders");
    std::shared_ptr<FluidModel> fluidModel = FluidModel::createSquare(glm::vec2(-100, 100), glm::vec2(0, 0), 5);
    
    std::shared_ptr<Simulation> simulation = std::make_shared<Simulation>();
    simulation->setSearchRadius(20);

    Simulation::setCurrentInstance(simulation);
    simulation->addFluidModel(fluidModel);
    simulation->kernelFct = [](double r, double h) {
        double q = r/h;
        double alfa = 15./(7*3.141*h*h);
        double res = 0;
        if (q < 1) {
            res = 2./3 - q*q + 1./2 * q * q * q; 
        } else if (q < 2) {
            double tmp = 2 - q;
            res = 1./6 * tmp * tmp * tmp;
        }
        return alfa * res;
    };

    std::cout<<simulation->kernelFct(10, 20)<<std::endl;
    simulation->smoothingLength = 10;
    

    while (!glfwWindowShouldClose(m_window)) {
        double currentTime = glfwGetTime();
       
        if (currentTime - m_lastTime >= 1.0/60.0){
            Simulation::getCurrentInstance()->step();

            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            Renderer2d::renderBoundingBox();
            Renderer2d::renderFluidModels();
            
            glfwSwapBuffers(m_window);

            glfwPollEvents();
            m_lastTime = currentTime;
        }
    }
    Renderer2d::cleanup();
};

void Application::closeWindow() {
    glfwSetWindowShouldClose(m_window, 1);
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
    glEnable(GL_PROGRAM_POINT_SIZE);
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
    if (key == GLFW_KEY_D) {
        Camera2D::move(glm::vec2(10, 0));
    } else if (key == GLFW_KEY_A) {
        Camera2D::move(glm::vec2(-10, 0));
    } else if (key == GLFW_KEY_W) {
        Camera2D::move(glm::vec2(0, 10));
    } else if (key == GLFW_KEY_S) {
        Camera2D::move(glm::vec2(0, -10));
    }
}