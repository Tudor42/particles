#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <imgui.h>
#include "external/imgui/imgui_impl_glfw.h"
#include "external/imgui/imgui_impl_opengl3.h"

GLFWwindow* WINDOW;

int main(void)
{
    if (!glfwInit())
    {
        std::exit(-1);
    }

    WINDOW = glfwCreateWindow(800, 600, "Particle-based simulation", NULL, NULL);
    if (!WINDOW)
    {
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(WINDOW);
    glewInit();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(WINDOW, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(WINDOW))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        glfwSwapBuffers(WINDOW);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

