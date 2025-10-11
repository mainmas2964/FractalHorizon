#include "ChunkRender.h"
#include <vector>
#include <iostream>

ChunkRenderer::ChunkRenderer()
    : vao_(0), vbo_(0), ibo_(0), indexCount_(0) {}

ChunkRenderer::~ChunkRenderer() {
    if (vao_) glDeleteVertexArrays(1, &vao_);
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (ibo_) glDeleteBuffers(1, &ibo_);
}

void ChunkRenderer::upload(const Chunk& chunk) {
    // free previous buffers if exist
    if (vao_) { glDeleteVertexArrays(1, &vao_); vao_ = 0; }
    if (vbo_) { glDeleteBuffers(1, &vbo_); vbo_ = 0; }
    if (ibo_) { glDeleteBuffers(1, &ibo_); ibo_ = 0; }

    std::vector<VoxelVertex> verts;
    std::vector<uint32_t> idx;
    chunk.buildMesh(verts, idx);

    if (verts.empty() || idx.empty()) {
        indexCount_ = 0;
        return;
    }

    indexCount_ = idx.size();

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);

    glBindVertexArray(vao_);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(VoxelVertex), verts.data(), GL_STATIC_DRAW);

    // IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(uint32_t), idx.data(), GL_STATIC_DRAW);

    // position at offset x
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, x));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, u));

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, light));
    // glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, light));

    glBindVertexArray(0);

    // unbind buffers (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ChunkRenderer::draw() const {
    if (!vao_ || indexCount_ == 0) return;
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
