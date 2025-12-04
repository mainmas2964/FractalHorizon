/*
created by mainmas2964  2025, 4 october
*/
#ifndef ENTITY_H
#define ENTITY_H
#include <cstdint>
struct Entity
{
    uint32_t id;
};
// basic entity structure

class EntityManager {
public:

    Entity createEntity();
    void destroyEntity(Entity e);

    Entity getEntityById(uint32_t id);
private:
    uint32_t nextEntityId = 0;
};
#endif