#include "Shader.h"
#include "utils/errors.h"
#include <fstream>
#include <sstream>

using namespace gui;

bool checkShaderCompilation(GLuint shader) {
    GLint status;
    GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
    if (status != GL_TRUE) {
        char buffer[512];
        GLCall(glGetShaderInfoLog(shader, 512, nullptr, buffer));
        std::cerr << "Shader compilation failed: " << buffer << std::endl;
    }
    return status;
}

bool checkProgramLinking(GLuint program) {
    GLint status;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &status));
    if (status != GL_TRUE) {
        char buffer[512];
        GLCall(glGetProgramInfoLog(program, 512, nullptr, buffer));
        std::cerr << "Program linking failed: " << buffer << std::endl;
    }
    return status;
}

Shader::Shader() {
    m_initialized = false;
    for (unsigned char i = 0; i < 3; i++) {
        m_shaders[i] = 0;
    }
    m_attributes.clear();
    m_uniforms.clear();
    m_program = 0;
}

Shader::~Shader(void) {
    m_initialized = false;
    m_attributes.clear();
    m_uniforms.clear();
    if (m_program) {
        GLCall(glDeleteProgram(m_program));
    }
}

void Shader::compileShaderString(GLenum type, const std::string &source) {
    GLCall(GLuint shader = glCreateShader(type));

    const GLchar *schr = source.c_str();

    GLCall(glShaderSource(shader, 1, &schr, NULL));

    GLCall(glCompileShader(shader));
    if (checkShaderCompilation(shader) == GL_TRUE) {
        if (type == GL_VERTEX_SHADER)
            m_shaders[0] = shader;
        else if (type == GL_GEOMETRY_SHADER)
            m_shaders[1] = shader;
        else if (type == GL_FRAGMENT_SHADER)
            m_shaders[2] = shader;
    }
}

void Shader::compileShaderFile(GLenum type, const std::string &filename) {
    std::ifstream fp;
    fp.open(filename.c_str(), std::ios_base::in);
    if(fp) {
        std::ostringstream output;
        output << fp.rdbuf();
        fp.close();

        compileShaderString(type, output.str());
    }
}

void Shader::createAndLinkProgram() {
    GLCall(m_program = glCreateProgram());
    for (unsigned char i = 0; i < 3; i++) {
        if (m_shaders[i] != 0) {
            GLCall(glAttachShader(m_program, m_shaders[i]));
        }
    }

    
    GLCall(glLinkProgram(m_program));
    
    if (checkProgramLinking(m_program) == GL_TRUE) {
        m_initialized = true;
    }
        
    for (unsigned char i = 0; i < 3; i++) {
        GLCall(glDeleteShader(m_shaders[i]));
    }
}

void Shader::bind() 
{
    if (m_initialized) {
        GLCall(glUseProgram(m_program));
    }
}

void Shader::unbind() 
{
    if (m_initialized) {
        GLCall(glUseProgram(0));
    }
}

void Shader::createAttribute(const std::string &attribute)
{
    GLCall(m_attributes[attribute] = glGetAttribLocation(m_program, attribute.c_str()));
}

GLuint Shader::getAttribute(const std::string &attribute)
{
    return m_attributes[attribute];
}

void Shader::createUniform(const std::string &uniform) {
    GLCall(m_uniforms[uniform] = glGetUniformLocation(m_program, uniform.c_str()));
}

GLuint Shader::getUniform(const std::string &uniform) {
    return m_uniforms[uniform];
}
