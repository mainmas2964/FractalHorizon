#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <cstdint>
#include <cassert>
#include "world/voxel/voxel.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16
#define CHUNK_SIZE (CHUNK_X * CHUNK_Y * CHUNK_Z)

struct VoxelVertex {
    float x, y, z;      // position
    float nx, ny, nz;   // normal (vec3)
    float u, v;         // uv
};
static_assert(sizeof(VoxelVertex) == 32, "VoxelVertex size must be 32");

class Chunk {
public:
    Chunk();
    ~Chunk();
    bool isLoaded() const;
    void setLoaded(bool loaded);
    void load();
    void unload();

    inline Voxel& at(int x, int y, int z) {
        assert(x >= 0 && x < CHUNK_X && y >= 0 && y < CHUNK_Y && z >= 0 && z < CHUNK_Z);
        return _voxels[(z * CHUNK_Y + y) * CHUNK_X + x];
    }
    inline const Voxel& at(int x, int y, int z) const {
        assert(x >= 0 && x < CHUNK_X && y >= 0 && y < CHUNK_Y && z >= 0 && z < CHUNK_Z);
        return _voxels[(z * CHUNK_Y + y) * CHUNK_X + x];
    }

    void buildMesh(std::vector<VoxelVertex>& outVerts, std::vector<uint32_t>& outIdx, int worldChunkX = 0, int worldChunkY = 0, int worldChunkZ = 0) const;

private:
    bool _isLoaded;
    std::vector<Voxel> _voxels;
};

#endif
