#pragma once
#include <GL/glew.h>
#include "utils/errors.h"
#include "BufferLayout.h"

namespace gui {
    class VertexBuffer {
    private:
        unsigned int m_RendererId;
        BufferLayout m_Layout;
        unsigned int m_size;
    public:
        VertexBuffer(void *data, unsigned int size);
        ~VertexBuffer();

        void bind() const;
        void unbind() const;

        const BufferLayout& get_layout() const { return m_Layout; };
		void set_layout(const BufferLayout& layout) { m_Layout = layout; };

        void SetData(const void* data, uint32_t size)
        {   
            glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
            if (size > m_size) {
                glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
                m_size = size;
                return;
            }
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        }

    };
}
