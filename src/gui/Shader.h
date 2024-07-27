#pragma once

#include <string>
#include <map>
#include <gl/glew.h>

namespace gui {
    class Shader {
    public:
        Shader();
        ~Shader();

        void compileShaderString(GLenum type, const std::string &source);
        void compileShaderFile(GLenum type, const std::string &filename);
        void createAndLinkProgram();
        
        void createAttribute(const std::string &attribute);
		void createUniform(const std::string &uniform);
		bool isInitialized();

		void bind();
		void unbind();

		GLuint getAttribute(const std::string &attribute);
		GLuint getUniform(const std::string &uniform);

	private:
	    bool m_initialized;
		GLuint	m_program;
	    std::map<std::string, GLuint> m_attributes;
	    std::map<std::string, GLuint> m_uniforms;
	    GLuint m_shaders[3];	
	};
};
