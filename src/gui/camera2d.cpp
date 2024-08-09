#include "Camera2D.h"
#include <algorithm>

glm::vec2 Camera2D::m_Position = glm::vec2(0, 0);
double Camera2D::m_Zoom = 1.;
bool Camera2D::m_ViewMatrixNeedsUpdate = true;
int Camera2D::m_ScreenHeight = 0;
int Camera2D::m_ScreenWidth = 0;
glm::mat4 Camera2D::m_ViewMatrix;

void Camera2D::setPosition(const glm::vec2& position) {
    m_Position = position;
    m_ViewMatrixNeedsUpdate = true;
}

void Camera2D::move(const glm::vec2 &velocity) {
    m_Position += velocity;
    m_ViewMatrixNeedsUpdate = true;
}

void Camera2D::zoom(double zoomOffset) {
    m_Zoom += zoomOffset;
    m_Zoom = std::clamp(m_Zoom, 0.1, 10.);

    m_ViewMatrixNeedsUpdate = true;
}


const glm::mat4& Camera2D::getViewMatrix() {
    if (m_ViewMatrixNeedsUpdate) {
        updateViewMatrix();
    }
    return m_ViewMatrix;
}

void Camera2D::updateViewMatrix() {
    // Translate by the negative position and scale by the zoom factor
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(-m_Position, 0.0f));
    transform = glm::scale(transform, glm::vec3(m_Zoom, m_Zoom, 1.0f));
    
    // Create an orthographic projection matrix with the screen dimensions
    glm::mat4 ortho = glm::ortho(-1.f * m_ScreenWidth / 2, 1.f * m_ScreenWidth / 2, 1.f * m_ScreenHeight / 2,  -1.f * m_ScreenHeight / 2, -1.0f, 1.0f);
    
    m_ViewMatrix = ortho * transform;
    m_ViewMatrixNeedsUpdate = false;
}
