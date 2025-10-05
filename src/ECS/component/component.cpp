/*
created by mainmas2964  2025, 5 octomber
*/
#include "component.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include "ECS/entity/entity.h"
#include <cstdint>

template<typename T>
void Component<T>::addComponent(Entity e, const T& component) {
    if (e >= sparse.size()) {
        throw std::out_of_range("Entity id exceeds maximum limit");
    }
    if (sparse[entity] != 0xFFFFFFFF) {
        throw std::runtime_error("Entity already has component");
    }
    sparse[entity] = comp.size();
    dense.push_back(entity);
    comp.push_back(component);
}
template<typename T>
void Component<T>::removeComponent(Entity e) {
    if (entity >= sparse.size() || sparse[Entity] == 0xFFFFFFFF)
        throw std::runtime_error("Entity does not have component");
    uint32_t index = sparse[entity]; 
    uint32_t lastEntity = dense.back();
    comp[index] = comp.back();
    dense[index] = lastEntity;
    sparse[lastEntity] = index;
    comp.pop_back();
    dense.pop_back();
    sparse[entity] = 0xFFFFFFFF;
}
template<typename T>
bool Component<T>::hasComponent(Entity e) {
    if (entity >= sparse.size()) return false;
    return sparse[entity] != 0xFFFFFFFF;
}
template<typename T>
T& Component<T>::getComponent(Entity e) {
    if (entity >= sparse.size() || sparse[entity] == 0xFFFFFFFF)
        throw std::runtime_error("Entity does not have component");
    return comp[sparse[entity]];
}