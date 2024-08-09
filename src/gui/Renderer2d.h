#pragma once
#include "gui/OpenGL/Shader.h"
#include "simulation/models/FluidModel.h"
#include "gui/camera2d.h"
#include "simulation/models/BoxBoundary.h"
#include "gui/OpenGL/VertexArray.h"

class Renderer2d {
private:
    static std::shared_ptr<gui::Shader> circleShader;
    static std::shared_ptr<gui::Shader> simple2DShader;
    static std::shared_ptr<gui::VertexArray> boundingBox;
    static std::shared_ptr<gui::VertexArray> particlesData;
public:
    static void init(const std::string &shaderPath);
    static void cleanup();

    static void renderFluidModels();
    static void renderBoundingBox();
};