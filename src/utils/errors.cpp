
#include "errors.h"
#include <GL/glew.h>

static void gui::GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}

static bool gui::GLLogCall(const char *function, const char *file, int line) {
    while(GLenum error = glGetError()){
        std::cerr << "[OpenGL ERROR] (" << error << ")" << function <<  
        " " << file << ": " << line << std::endl;
        return false;
    }
    return true;
}