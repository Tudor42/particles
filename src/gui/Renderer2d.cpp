#include "Renderer2d.h"
#include "gui/OpenGL/VertexBuffer.h"
#include "gui/OpenGL/IndexBuffer.h"
#include <glm/gtc/type_ptr.hpp>
#include "simulation/Simulation.h"

std::shared_ptr<gui::Shader> Renderer2d::circleShader;
std::shared_ptr<gui::Shader> Renderer2d::simple2DShader;
std::shared_ptr<gui::VertexArray> Renderer2d::boundingBox;
std::shared_ptr<gui::VertexArray> Renderer2d::particlesData;

void Renderer2d::init(const std::string &shaderPath) {
    circleShader = std::make_shared<gui::Shader>();
    circleShader->compileShaderFile(GL_VERTEX_SHADER, shaderPath + "/render2D_circle.vert");
    circleShader->compileShaderFile(GL_FRAGMENT_SHADER, shaderPath + "/render2D_circle.frag"); 
    circleShader->createAndLinkProgram();
    circleShader->createUniform("pointSize");
    circleShader->createUniform("view");

    std::shared_ptr<gui::VertexBuffer> positions = std::make_shared<gui::VertexBuffer>(nullptr, 0);
    positions->set_layout({
        { gui::ShaderDataType::Float3, "a_Position" },
        //{ gui::ShaderDataType::Float4, "a_Color" }
    });
    std::shared_ptr<gui::VertexBuffer> color = std::make_shared<gui::VertexBuffer>(nullptr, 0);
    color->set_layout({
        { gui::ShaderDataType::Float4, "a_Color" }
    });

    particlesData = std::make_shared<gui::VertexArray>();
    particlesData->add_vertex_buffer(positions);
    particlesData->add_vertex_buffer(color);

    simple2DShader = std::make_shared<gui::Shader>();
    simple2DShader->compileShaderFile(GL_VERTEX_SHADER, shaderPath + "/simple2D.vert");
    simple2DShader->compileShaderFile(GL_FRAGMENT_SHADER, shaderPath + "/simple2D.frag");
    simple2DShader->createAndLinkProgram();
    simple2DShader->createUniform("view");

    std::shared_ptr<gui::VertexBuffer> boxVertices = std::make_shared<gui::VertexBuffer>(nullptr, 0);
    boxVertices->set_layout({
        { gui::ShaderDataType::Float2, "a_Position" }
    });
    
    boundingBox = std::make_shared<gui::VertexArray>();
    boundingBox->add_vertex_buffer(boxVertices);

    std::shared_ptr<gui::IndexBuffer> boxIndexes = std::make_shared<gui::IndexBuffer>();
    unsigned int indices[] = { 0, 1, 2, 3 };
    boxIndexes->set_data(indices, 4);
    boundingBox->set_index_buffer(boxIndexes);
}

void Renderer2d::cleanup() {
    circleShader = nullptr;
    simple2DShader = nullptr;
    boundingBox = nullptr;
    particlesData = nullptr;
}

void Renderer2d::renderFluidModels() {
    particlesData->bind();
    circleShader->bind();
    GLCall(glUniformMatrix4fv(circleShader->getUniform("view"), 1, GL_FALSE, glm::value_ptr(Camera2D::getViewMatrix())));
    
    for(auto &fluidModel: Simulation::getCurrentInstance()->getFluidModels()){
        if (fluidModel->getNumParticles() == 0) {
            continue;
        }
        particlesData->get_vertex_buffers()[0]->SetData(glm::value_ptr(fluidModel->getPosition(0)), fluidModel->getNumParticles() * 3 * sizeof(float));
        particlesData->get_vertex_buffers()[1]->SetData(glm::value_ptr(fluidModel->getColor(0)), fluidModel->getNumParticles() * 4 * sizeof(float));        
        
        GLCall(glUniform1f(circleShader->getUniform("pointSize"), ((float)(Camera2D::getZoom() * 11./12 * fluidModel->getRadius()))));

        GLCall(glDrawArrays(GL_POINTS, 0, fluidModel->getNumParticles()));
    }

}

void Renderer2d::renderBoundingBox() {
    glLineWidth((float)(Camera2D::getZoom() * 1.0));
    simple2DShader->bind();

    glUniformMatrix4fv(simple2DShader->getUniform("view"), 1, GL_FALSE, glm::value_ptr(Camera2D::getViewMatrix()));
    glm::vec2 lower = BoxBoundary::lowerBoundary;
    glm::vec2 upper = BoxBoundary::upperBoundary;
    float tmp[] = {lower.x, lower.y, lower.x, upper.y, upper.x, upper.y, upper.x, lower.y};
    auto tmpvert = boundingBox->get_vertex_buffers()[0];
    tmpvert->SetData(tmp, sizeof(float) * 8);
    boundingBox->bind();

    GLCall(glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0));
}