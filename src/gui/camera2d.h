#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D {
public:
    static void setWidth(const int width) { m_ScreenWidth = width; m_ViewMatrixNeedsUpdate = true; }
    static void setHeight(const int height) { m_ScreenHeight = height; m_ViewMatrixNeedsUpdate = true; }
    
    static void setPosition(const glm::vec2& position);
    static void move(const glm::vec2& velocity);

    static void zoom(double zoomOffset);

    static double getZoom() { return m_Zoom; }
    static const glm::mat4& getViewMatrix();

private:
    static void updateViewMatrix();

    static glm::vec2 m_Position;
    static double m_Zoom;
    static int m_ScreenWidth, m_ScreenHeight;
    static glm::mat4 m_ViewMatrix;
    static bool m_ViewMatrixNeedsUpdate;
};