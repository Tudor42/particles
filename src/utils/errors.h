#pragma once

#include <iostream>
#include "extern/debugging/debugging-traps.h"

#define ASSERT(x) if (!(x)) psnip_dbg_assert(x)
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

namespace gui {
    static void GLClearError();  
    static bool GLLogCall(const char *function, const char *file, int line);
}