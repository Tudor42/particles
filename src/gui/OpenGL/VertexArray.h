#pragma once

#include "VertexBuffer.h"
#include <vector>
#include "IndexBuffer.h"

namespace gui {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void bind() const;
        void unbind() const;

        void add_vertex_buffer(VertexBuffer& vertexBuffer);
        void set_index_buffer(const IndexBuffer& indexBuffer);

        const std::vector<VertexBuffer*>& get_vertex_buffers() const { return m_VertexBuffers; }
        const IndexBuffer& get_index_buffer() const { return m_IndexBuffer; }
    private:
        uint32_t m_RendererID;
        uint32_t m_VertexBufferIndex = 0;
        std::vector<VertexBuffer*> m_VertexBuffers;
        IndexBuffer m_IndexBuffer;
    };
}