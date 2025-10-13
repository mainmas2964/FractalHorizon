#include "ChunkRender.h"
#include "world/chunk/chunk.h"
#include <vector>
#include <iostream>
#include <cstddef>
#include <algorithm>

ChunkRenderer::ChunkRenderer()
    : vao_(0), vbo_(0), ibo_(0), indexCount_(0) {}

ChunkRenderer::~ChunkRenderer() {
    if (vao_) { glDeleteVertexArrays(1, &vao_); vao_ = 0; }
    if (vbo_) { glDeleteBuffers(1, &vbo_); vbo_ = 0; }
    if (ibo_) { glDeleteBuffers(1, &ibo_); ibo_ = 0; }
}

void ChunkRenderer::upload(const Chunk& chunk) {
    if (vao_) { glDeleteVertexArrays(1, &vao_); vao_ = 0; }
    if (vbo_) { glDeleteBuffers(1, &vbo_); vbo_ = 0; }
    if (ibo_) { glDeleteBuffers(1, &ibo_); ibo_ = 0; }

    std::vector<VoxelVertex> verts;
    std::vector<uint32_t> idx;
    chunk.buildMesh(verts, idx);

    for (size_t i = 0; i < std::min<size_t>(verts.size(), 8); ++i) {
        std::cout << "V v" << i << " pos=("
                  << verts[i].x << "," << verts[i].y << "," << verts[i].z << ")"
                  << " normal=(" << verts[i].nx << "," << verts[i].ny << "," << verts[i].nz << ")"
                  << " uv=(" << verts[i].u << "," << verts[i].v << ")\n";
    }

    if (verts.empty() || idx.empty()) {
        indexCount_ = 0;
        return;
    }

    indexCount_ = idx.size();

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(VoxelVertex), verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(uint32_t), idx.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, x));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, u));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), (void*)offsetof(VoxelVertex, nx));

    std::cout << "VoxelVertex sizeof=" << sizeof(VoxelVertex) << "\n";

    GLint size = 0, type = 0, stride = 0;
    glGetVertexAttribiv(2, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
    glGetVertexAttribiv(2, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
    glGetVertexAttribiv(2, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
    std::cout << "attrib2 size=" << size << " type=0x" << std::hex << type << std::dec << " stride=" << stride << "\n";

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ChunkRenderer::draw() const {
    if (!vao_ || indexCount_ == 0) return;
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
