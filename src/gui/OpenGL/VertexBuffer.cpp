#include "VertexBuffer.h"
#include "utils/errors.h"

gui::VertexBuffer::VertexBuffer(void *data, unsigned int size) : m_size(size)
{
    GLCall(glGenBuffers(1, &m_RendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

gui::VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererId));
}

void gui::VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void gui::VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
