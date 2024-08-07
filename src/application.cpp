#include "./application.h"
#include "gui/OpenGL/Shader.h"
#include <glm/glm.hpp>
#include "gui/OpenGL/IndexBuffer.h"
#include "gui/OpenGL/VertexArray.h"
#include "gui/OpenGL/VertexBuffer.h"

Application *Application::instance = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void Application::run() {
    gui::VertexArray vertexArray;
    float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f, 1.0f
	};
    gui::VertexBuffer vertexBuffer(vertices, sizeof(vertices));
    vertexBuffer.set_layout({
        { gui::ShaderDataType::Float3, "a_Position" },
		{ gui::ShaderDataType::Float4, "a_Color" }
    });
    vertexArray.add_vertex_buffer(vertexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };

    gui::IndexBuffer indexBuffer;
    indexBuffer.set_data(indices, 3);
    vertexArray.set_index_buffer(indexBuffer);

    gui::Shader shader;
    shader.compileShaderFile(GL_FRAGMENT_SHADER, "resources/shaders/testshader.frag");
    shader.compileShaderFile(GL_VERTEX_SHADER, "resources/shaders/testshader.vert");
    shader.createAndLinkProgram();
    
    while (!glfwWindowShouldClose(m_window)) {
        double currentTime = glfwGetTime();
       
        if (currentTime - m_lastTime >= 1.0/60.0){
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);
            
            shader.bind();
            vertexArray.bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(m_window);

            m_lastTime = currentTime;
        }
    }
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
    
	glfwGetFramebufferSize(m_window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    //glClearColor(CLEAR_COLOR);
    glEnable(GL_PROGRAM_POINT_SIZE);
    //glEnable(GL_POINT_SPRITE);
}

Application::~Application() {
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->onFramebufferSizeChanged(width, height);
    }
}
