#pragma once

namespace gui {
    class Renderer {
    public:
        void drawSphere() const;
        void drawPoint() const;
        void drawMesh() const;

        void clear() const;
    };
}