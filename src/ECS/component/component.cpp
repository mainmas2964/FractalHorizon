/*
created by mainmas2964  2025, 5 octomber
*/
#include "component.h"
#include <stdexcept>
#include <cstdint>
#include <limits>
#include <utility> // std::move
#include "ECS/entity/entity.h"
template<typename T>
void Component<T>::addComponent(Entity e, const T& component) {
    uint32_t entity_id = e.id;
    assert(entity_id < sparse.size() && "Entity ID exceeds maximum entities");
    if (sparse[entity_id] != 0xFFFFFFFF) {
        throw std::runtime_error("Entity already has this component");
    }
    sparse[entity_id] = static_cast<uint32_t>(comp.size());
    dense.push_back(entity_id);
    comp.push_back(component);
}
template<typename T>
void Component<T>::removeComponent(Entity e) {
    uint32_t entity_id = e.id;
    assert(entity_id < sparse.size() && "Entity ID exceeds maximum entities");
    uint32_t index = sparse[entity_id];
    if (index == 0xFFFFFFFF) {
        throw std::runtime_error("Entity does not have this component");
    }
    uint32_t last_entity_id = dense.back();
    comp[index] = std::move(comp.back());
    dense[index] = last_entity_id;
    sparse[last_entity_id] = index;
    comp.pop_back();
    dense.pop_back();
    sparse[entity_id] = 0xFFFFFFFF; // mark as removed
}
template<typename T>
bool Component<T>::hasComponent(Entity e) {
    uint32_t entity_id = e.id;
    assert(entity_id < sparse.size() && "Entity ID exceeds maximum entities");
    return sparse[entity_id] != 0xFFFFFFFF;
}
template<typename T>
T& Component<T>::getComponent(Entity e) {
    uint32_t entity_id = e.id;
    assert(entity_id < sparse.size() && "Entity ID exceeds maximum entities");
    uint32_t index = sparse[entity_id];
    if (index == 0xFFFFFFFF) {
        throw std::runtime_error("Entity does not have this component");
    }
    return comp[index];
}
// explicit template instantiation for common component types
template class Component<int>;
template class Component<float>;
template class Component<double>;
template class Component<std::string>;
// add more types as needed