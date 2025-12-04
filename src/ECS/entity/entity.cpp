#include "entity.h"
Entity EntityManager::createEntity() {
    Entity e{ nextEntityId++ };
    return e;
}
void EntityManager::destroyEntity(Entity e) {}
Entity EntityManager::getEntityById(uint32_t id) {
    return Entity{ id };
}
