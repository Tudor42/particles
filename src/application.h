#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include "./utils/errors.h"

#define CLEAR_COLOR 0.5f, 0.5f, 0.5f, 1.0f

class Application {
public:
    int getWidth() { m_width; };
    int getHeight() { m_height; };
    void run();
    void closeWindow();
    void onFramebufferSizeChanged(int width, int height) {
        glViewport(0, 0, width, height);
        m_width = width;
        m_height = height;
    }
    static Application *getInstance() {
        if (instance) {
            return instance;
        }
        return (instance = new Application(800, 600));
    }
private:
    static Application *instance;
    Application(int width, int height);
    ~Application();
    GLFWwindow* m_window;
    double m_lastTime = 0;
    int m_width, m_height;
};

