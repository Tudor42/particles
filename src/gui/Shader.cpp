#include "Shader.h"

using namespace gui;

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
	if (m_program)
		glDeleteProgram(m_program);
}