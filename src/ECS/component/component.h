/*
created by mainmas2964  2025, 4 octomber
*/
#ifndef COMPONENT_H
#define COMPONENT_H
#include <vector>
#include <cassert>
#include <cstdint>
#include "ECS/entity/entity.h"
template<typename T>
class Component
{
public:
    std::vector<T> comp;
    std::vector<uint32_t> dense; //entities
    std::vector<uint32_t> sparse; //sparse[entity_id] -> index in the dense
    Component(size_t maxEntities){
        sparse.resize(maxEntities,0xFFFFFFFF); // initialize sparse array with invalid indices (0xFFFFFFFF)
    };
    void addComponent(Entity e, const T& component); //add component to entity
    void removeComponent(Entity e); //remove component from entity
    bool hasComponent(Entity e); //check if entity has component
    T& getComponent(Entity e); //get component of entity
    size_t size() const { return comp.size(); } //get number of components
};
#endif