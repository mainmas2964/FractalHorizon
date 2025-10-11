#ifndef VOXEL_H
#define VOXEL_H
#include <cstdint>

struct Voxel {
    uint32_t id; // заготовка для типов вокселей
    inline bool isAir() const { return id == 0u; }
};

#endif // VOXEL_H
