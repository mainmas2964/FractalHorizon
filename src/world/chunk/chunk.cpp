#include "chunk.h"
#include <vector>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <random>
#include <iostream>

static const int FACE_COUNT = 6;
static const int FACE_POS[6][4][3] = {
    { {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1} },
    { {0,0,1}, {0,1,1}, {0,1,0}, {0,0,0} },
    { {0,1,1}, {1,1,1}, {1,1,0}, {0,1,0} },
    { {0,0,0}, {1,0,0}, {1,0,1}, {0,0,1} },
    { {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1} },
    { {1,0,0}, {0,0,0}, {0,1,0}, {1,1,0} }
};

static const float FACE_NORMALS[6][3] = {
    { 1.0f,  0.0f,  0.0f},
    {-1.0f,  0.0f,  0.0f},
    { 0.0f,  1.0f,  0.0f},
    { 0.0f, -1.0f,  0.0f},
    { 0.0f,  0.0f,  1.0f},
    { 0.0f,  0.0f, -1.0f}
};

static const float FACE_UV[4][2] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f}
};

static bool voxelIsZero(const Voxel& v) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&v);
    for (size_t i = 0; i < sizeof(Voxel); ++i) if (p[i] != 0) return false;
    return true;
}

static void voxelSetZero(Voxel& v) { std::memset(&v, 0, sizeof(Voxel)); }
static void voxelSetNonZero(Voxel& v) { unsigned char* p = reinterpret_cast<unsigned char*>(&v); p[0] = 1; for (size_t i = 1; i < sizeof(Voxel); ++i) p[i] = 0; }

Chunk::Chunk() {
    _voxels.resize(CHUNK_X * CHUNK_Y * CHUNK_Z);
    for (auto &v : _voxels) voxelSetZero(v);
    _isLoaded = false;
}

Chunk::~Chunk() = default;

bool Chunk::isLoaded() const { return _isLoaded; }
void Chunk::setLoaded(bool loaded) { _isLoaded = loaded; }

void Chunk::load() {
    for (auto &v : _voxels) voxelSetZero(v);

    // random fill: roughly 25% occupancy by default
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    const float fillProbability = 0.25f;

    for (int x = 0; x < CHUNK_X; ++x) {
        for (int y = 0; y < CHUNK_Y; ++y) {
            for (int z = 0; z < CHUNK_Z; ++z) {
                if (dist(rng) < fillProbability) {
                    size_t idx = static_cast<size_t>((z * CHUNK_Y + y) * CHUNK_X + x);
                    assert(idx < _voxels.size());
                    voxelSetNonZero(_voxels[idx]);
                }
            }
        }
    }

    _isLoaded = true;

    size_t solidCount = 0;
    for (const auto &v : _voxels) if (!voxelIsZero(v)) ++solidCount;
    std::cout << "[Chunk] load: solid voxels = " << solidCount << "\n";
}

void Chunk::unload() {
    for (auto &v : _voxels) voxelSetZero(v);
    _isLoaded = false;
}

void Chunk::buildMesh(std::vector<VoxelVertex>& outVerts, std::vector<uint32_t>& outIdx, int /*worldChunkX*/, int /*worldChunkY*/, int /*worldChunkZ*/) const {
    outVerts.clear();
    outIdx.clear();
    outVerts.reserve(4096);
    outIdx.reserve(8192);

    size_t solidCount = 0;
    for (int x = 0; x < CHUNK_X; ++x)
        for (int y = 0; y < CHUNK_Y; ++y)
            for (int z = 0; z < CHUNK_Z; ++z) {
                size_t idx = static_cast<size_t>((z * CHUNK_Y + y) * CHUNK_X + x);
                if (!voxelIsZero(_voxels[idx])) ++solidCount;
            }
    std::cout << "[Chunk] buildMesh: solidCount=" << solidCount << "\n";

    auto isSolidLocal = [this](int x, int y, int z) -> bool {
        if (x < 0 || x >= CHUNK_X || y < 0 || y >= CHUNK_Y || z < 0 || z >= CHUNK_Z) return false;
        size_t idx = static_cast<size_t>((z * CHUNK_Y + y) * CHUNK_X + x);
        assert(idx < _voxels.size());
        return !voxelIsZero(_voxels[idx]);
    };

    for (int x = 0; x < CHUNK_X; ++x) {
        for (int y = 0; y < CHUNK_Y; ++y) {
            for (int z = 0; z < CHUNK_Z; ++z) {
                if (!isSolidLocal(x,y,z)) continue;

                for (int f = 0; f < FACE_COUNT; ++f) {
                    int nx = x, ny = y, nz = z;
                    switch (f) {
                        case 0: nx = x + 1; break;
                        case 1: nx = x - 1; break;
                        case 2: ny = y + 1; break;
                        case 3: ny = y - 1; break;
                        case 4: nz = z + 1; break;
                        case 5: nz = z - 1; break;
                    }
                    if (isSolidLocal(nx,ny,nz)) continue;

                    uint32_t baseIndex = static_cast<uint32_t>(outVerts.size());
                    const float* nrm = FACE_NORMALS[f];

                    for (int vi = 0; vi < 4; ++vi) {
                        VoxelVertex vv;
                        vv.x = static_cast<float>(x) + FACE_POS[f][vi][0];
                        vv.y = static_cast<float>(y) + FACE_POS[f][vi][1];
                        vv.z = static_cast<float>(z) + FACE_POS[f][vi][2];
                        vv.nx = nrm[0];
                        vv.ny = nrm[1];
                        vv.nz = nrm[2];
                        vv.u = FACE_UV[vi][0];
                        vv.v = FACE_UV[vi][1];
                        outVerts.push_back(vv);
                    }

                    outIdx.push_back(baseIndex + 0);
                    outIdx.push_back(baseIndex + 1);
                    outIdx.push_back(baseIndex + 2);
                    outIdx.push_back(baseIndex + 0);
                    outIdx.push_back(baseIndex + 2);
                    outIdx.push_back(baseIndex + 3);
                }
            }
        }
    }

    std::cout << "[Chunk] buildMesh: verts=" << outVerts.size() << " idx=" << outIdx.size() << "\n";
}

