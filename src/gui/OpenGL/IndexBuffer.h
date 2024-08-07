#pragma once
#include <GL/glew.h>

namespace gui {
    class IndexBuffer {
    private:
        unsigned int m_RendererId;
        unsigned int m_Count = 0;
    public:
        IndexBuffer();
        ~IndexBuffer();

        void bind() const;
        void unbind() const;
        void set_data(const unsigned int *data, unsigned int count);
        inline unsigned int getCount() const { return m_Count;}
    };
}

