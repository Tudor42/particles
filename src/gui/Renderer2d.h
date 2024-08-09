#pragma once
#include "gui/OpenGL/Shader.h"
#include "simulation/models/FluidModel.h"
#include "gui/camera2d.h"
#include "simulation/models/BoxBoundary.h"
#include "gui/OpenGL/VertexArray.h"

class Renderer2d {
private:
    static gui::Shader *circleShader;
    static gui::Shader *simple2DShader;
    static gui::VertexArray *boundingBox;
    static gui::VertexArray *circlesData;
public:
    static void initShaders(const std::string &shaderPath);

    static void renderFluid(FluidModel *model);
    static void renderBoundingBox();
};