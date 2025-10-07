#include "chunk.h"
#include <cstring> 
#include <iostream>
#include <vector>
#include "world/voxel/voxel.h"
#include <array>
Chunk::Chunk(){
    _isLoaded = false;
}
Chunk::~Chunk() {
    if (_isLoaded) {
        unload();
    }
}
bool Chunk::isLoaded() const {
    return _isLoaded;
}
void Chunk::setLoaded(bool loaded) {
    _isLoaded = loaded;
}
void Chunk::load() {
    if (_isLoaded) {
        std::cerr << "[Chunk] Warning: Chunk is already loaded\n";
        return;
    } else {
        voxel* voxels[CHUNK_X][CHUNK_Y][CHUNK_Z];
        for (int x = 0; x < CHUNK_X; ++x) {
            for (int y = 0; y < CHUNK_Y; ++y) {
                for (int z = 0; z < CHUNK_Z; ++z) {
                    //TODO: placeholder code just to fill chunk with some data
                }}}
    }
}
void Chunk::unload() {
    if (!_isLoaded) {
        std::cerr << "[Chunk] Warning: Chunk is not loaded\n";
        return;
    } else {
        Chunk::setLoaded(false);
    }
}
