#pragma once
#include "Shader.h"

namespace gui
{
    class Material {
    public:
        Material(Shader shader);
        ~Material();
        
    private: 
        Shader shader;
    };
} 
