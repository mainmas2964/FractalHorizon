#pragma once
#include <vector>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include "ECS/entity/entity.h"

template<typename T>
class Component
{
public:
    std::vector<T> comp;
    std::vector<uint32_t> dense;
    std::vector<uint32_t> sparse;

    Component(size_t maxEntities){
        sparse.resize(maxEntities, 0xFFFFFFFF);
    }

    void addComponent(Entity e, const T& component) {
        uint32_t entity_id = e.id;
        if (entity_id >= sparse.size()) throw std::runtime_error("entity ID exceeds maximum entities");
        if (sparse[entity_id] != 0xFFFFFFFF) throw std::runtime_error("entity already has this component");

        sparse[entity_id] = static_cast<uint32_t>(comp.size());
        dense.push_back(entity_id);
        comp.push_back(component);
    }

    void removeComponent(Entity e) {
        uint32_t entity_id = e.id;
        if (entity_id >= sparse.size()) throw std::runtime_error("entity ID exceeds maximum entities");
        uint32_t index = sparse[entity_id];
        if (index == 0xFFFFFFFF) return;
        uint32_t last_entity_id = dense.back();
        comp[index] = std::move(comp.back());
        dense[index] = last_entity_id;
        sparse[last_entity_id] = index;
        comp.pop_back();
        dense.pop_back();
        sparse[entity_id] = 0xFFFFFFFF;
    }

    bool hasComponent(Entity e) {
        uint32_t entity_id = e.id;
        if (entity_id >= sparse.size()) return false;
        return sparse[entity_id] != 0xFFFFFFFF;
    }

    T& getComponent(Entity e) {
        uint32_t entity_id = e.id;
        if (entity_id >= sparse.size()) throw std::runtime_error("entity ID exceeds maximum entities");
        uint32_t index = sparse[entity_id];
        if (index == 0xFFFFFFFF) throw std::runtime_error("entity does not have this component");
        return comp[index];
    }

    size_t size() const { return comp.size(); }
};
