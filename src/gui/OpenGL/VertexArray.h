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

        void add_vertex_buffer(std::shared_ptr<VertexBuffer>& vertexBuffer);
        void set_index_buffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

        const std::vector<std::shared_ptr<VertexBuffer>>& get_vertex_buffers() const { return m_VertexBuffers; }
        const IndexBuffer& get_index_buffer() const { return *m_IndexBuffer; }
    private:
        uint32_t m_RendererID;
        uint32_t m_VertexBufferIndex = 0;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}