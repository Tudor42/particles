#include "IndexBuffer.h"
#include "utils/errors.h"

gui::IndexBuffer::IndexBuffer() 
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint), "Unsigned int different size than GLuint");

    GLCall(glGenBuffers(1, &m_RendererId));
}

gui::IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererId));
}

void gui::IndexBuffer::set_data(const unsigned int *data, unsigned int count) {
    m_Count = count;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
}

void gui::IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void gui::IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
