#ifndef VOXEL_H
#define VOXEL_H

#include <cstdint>

enum class BlockType : uint8_t {
    Air   = 0,
    Solid = 1,
    // можно добавить другие типы dirt = 2, grass = 3 и тд
};

struct Voxel {
    uint8_t type;

    Voxel() : type(static_cast<uint8_t>(BlockType::Air)) {}
    explicit Voxel(BlockType t) : type(static_cast<uint8_t>(t)) {}

    bool isAir() const { return type == static_cast<uint8_t>(BlockType::Air); }
    BlockType getType() const { return static_cast<BlockType>(type); }
    void setType(BlockType t) { type = static_cast<uint8_t>(t); }
};

#endif
