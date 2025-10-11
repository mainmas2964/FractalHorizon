#pragma once
#include "world/chunk/chunk.h"
#include "glad/glad.h"
#include <cstddef>

class ChunkRenderer {
public:
    ChunkRenderer();
    ~ChunkRenderer();

    void upload(const Chunk& chunk);
    void draw() const;

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ibo_;
    size_t indexCount_;
};
