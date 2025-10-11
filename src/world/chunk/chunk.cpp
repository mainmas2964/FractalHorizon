#include "chunk.h"
#include <cstring>
#include <iostream>

// directions + face tables
static const int DIRS[6][3] = {
    {1,0,0}, {-1,0,0},
    {0,1,0}, {0,-1,0},
    {0,0,1}, {0,0,-1}
};

static const float FACE_POS[6][4][3] = {
    {{1,0,0},{1,1,0},{1,0,1},{1,1,1}},
    {{0,0,1},{0,1,1},{0,0,0},{0,1,0}},
    {{0,1,1},{1,1,1},{0,1,0},{1,1,0}},
    {{0,0,0},{1,0,0},{0,0,1},{1,0,1}},
    {{0,0,1},{1,0,1},{0,1,1},{1,1,1}},
    {{1,0,0},{0,0,0},{1,1,0},{0,1,0}}
};

static const float FACE_UV[4][2] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f}
};

static const uint32_t FACE_IDX[6] = {0,1,2, 2,1,3};

Chunk::Chunk() : _isLoaded(false) {
    _voxels.resize(CHUNK_SIZE);
}

Chunk::~Chunk() {
    if (_isLoaded) unload();
}

bool Chunk::isLoaded() const { return _isLoaded; }
void Chunk::setLoaded(bool loaded) { _isLoaded = loaded; }

void Chunk::load() {
    if (_isLoaded) {
        std::cerr << "[Chunk] Warning: already loaded\n";
        return;
    }
    for (int x = 0; x < CHUNK_X; ++x) {
        for (int y = 0; y < CHUNK_Y; ++y) {
            for (int z = 0; z < CHUNK_Z; ++z) {
                blocks[x][y][z] = (rand() % 100 < 30) ? BlockType::Solid : BlockType::Air;
            }
        }
    }
}

void Chunk::unload() {
    if (!_isLoaded) {
        std::cerr << "[Chunk] Warning: not loaded\n";
        return;
    }
    _voxels.clear();
    _voxels.resize(CHUNK_SIZE);
    _isLoaded = false;
}

void Chunk::buildMesh(std::vector<VoxelVertex>& outVerts, std::vector<uint32_t>& outIdx, int worldChunkX, int worldChunkY, int worldChunkZ) const {
    outVerts.clear();
    outIdx.clear();

    outVerts.reserve(4096);
    outIdx.reserve(8192);

    for (int z = 0; z < CHUNK_Z; ++z)
    for (int y = 0; y < CHUNK_Y; ++y)
    for (int x = 0; x < CHUNK_X; ++x) {
        const Voxel& v = at(x,y,z);
        if (v.isAir()) continue;

        for (int f = 0; f < 6; ++f) {
            int nx = x + DIRS[f][0];
            int ny = y + DIRS[f][1];
            int nz = z + DIRS[f][2];
            bool neighborOpaque = false;
            if (nx >= 0 && nx < CHUNK_X && ny >= 0 && ny < CHUNK_Y && nz >= 0 && nz < CHUNK_Z) {
                neighborOpaque = !at(nx,ny,nz).isAir();
            } else {
                neighborOpaque = false;
            }
            if (neighborOpaque) continue;

            uint32_t base = (uint32_t)outVerts.size();
            float ox = float(x + worldChunkX * CHUNK_X);
            float oy = float(y + worldChunkY * CHUNK_Y);
            float oz = float(z + worldChunkZ * CHUNK_Z);

uint32_t light = 255; // освещение как по граням 

switch (f) {
    case 4: light = 80; break;   // +Z
    case 5: light = 120; break;  // -Z
    case 0: case 1: light = 180; break; // +X
    case 2: case 3: light = 255; break; // +Y
}
            for (int vi = 0; vi < 4; ++vi) {
                VoxelVertex vv;
                vv.x = ox + FACE_POS[f][vi][0];
                vv.y = oy + FACE_POS[f][vi][1];
                vv.z = oz + FACE_POS[f][vi][2];
                vv.u = FACE_UV[vi][0];
                vv.v = FACE_UV[vi][1];
            //  vv.light = 255;
                vv.light = light;
                vv._pad[0] = vv._pad[1] = vv._pad[2] = 0;
                outVerts.push_back(vv);
            }

            outIdx.push_back(base + FACE_IDX[0]);
            outIdx.push_back(base + FACE_IDX[1]);
            outIdx.push_back(base + FACE_IDX[2]);
            outIdx.push_back(base + FACE_IDX[3]);
            outIdx.push_back(base + FACE_IDX[4]);
            outIdx.push_back(base + FACE_IDX[5]);
        }
    }
}
