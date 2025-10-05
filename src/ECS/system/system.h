/*
created by mainmas2964  2025, 4 octomber


*/
#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <cstdint>
#include "ECS/component/component.h"
#include "ECS/entity/entity.h"

template<typename T>
class System {
public:
    System(Component<T>& comp) : component(comp) {}

    template<typename Func>
    void update(Func&& func, int8_t iteration_step = 1); //update ALL components with a function and an iteration step

    template<typename Func>
    void updateSingleComp(Func&& func, Entity e); // update single component with a function

private:
    Component<T>& component;
};

#endif
