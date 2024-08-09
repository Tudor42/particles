#include "Renderer2d.h"
#include "gui/OpenGL/VertexBuffer.h"
#include "gui/OpenGL/IndexBuffer.h"
#include <glm/gtc/type_ptr.hpp>

gui::Shader * Renderer2d::circleShader;
gui::Shader * Renderer2d::simple2DShader;
gui::VertexArray * Renderer2d::boundingBox;
gui::VertexArray * Renderer2d::circlesData;

void Renderer2d::initShaders(const std::string &shaderPath) {
    circleShader = new gui::Shader();
    circleShader->compileShaderFile(GL_VERTEX_SHADER, shaderPath + "/render2D_circle.vert");
    circleShader->compileShaderFile(GL_FRAGMENT_SHADER, shaderPath + "/render2D_circle.frag"); 
    circleShader->createAndLinkProgram();
    circleShader->createUniform("pointSize");
    circleShader->createUniform("view");

    simple2DShader = new gui::Shader();
    simple2DShader->compileShaderFile(GL_VERTEX_SHADER, shaderPath + "/simple2D.vert");
    simple2DShader->compileShaderFile(GL_FRAGMENT_SHADER, shaderPath + "/simple2D.frag");
    simple2DShader->createAndLinkProgram();
    simple2DShader->createUniform("view");
    glm::vec2 lower = BoxBoundary::lowerBoundary;
    glm::vec2 upper = BoxBoundary::upperBoundary;
    std::shared_ptr<gui::VertexBuffer> boxVertices = std::make_shared<gui::VertexBuffer>(nullptr, 0);
    boxVertices->set_layout({
        { gui::ShaderDataType::Float2, "a_Position" }
    });
    
    boundingBox = new gui::VertexArray();
    boundingBox->add_vertex_buffer(boxVertices);

    std::shared_ptr<gui::IndexBuffer> boxIndexes = std::make_shared<gui::IndexBuffer>();
    unsigned int indices[] = { 0, 1, 2, 3 };
    boxIndexes->set_data(indices, 4);
    boundingBox->set_index_buffer(boxIndexes);
}

void Renderer2d::renderFluid(FluidModel *model) {
    
}

void Renderer2d::renderBoundingBox() {
    glLineWidth(Camera2D::getZoom() * 2.0);
    simple2DShader->bind();

    glUniformMatrix4fv(simple2DShader->getUniform("view"), 1, GL_FALSE, glm::value_ptr(Camera2D::getViewMatrix()));
    glm::vec2 lower = BoxBoundary::lowerBoundary;
    glm::vec2 upper = BoxBoundary::upperBoundary;
    float tmp[] = {lower.x, lower.y, lower.x, upper.y, upper.x, upper.y, upper.x, lower.y};
    auto tmpvert = boundingBox->get_vertex_buffers()[0];
    tmpvert->SetData(tmp, sizeof(float) * 8);
    boundingBox->bind();

    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
}